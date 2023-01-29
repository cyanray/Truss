#include "Truss/Solver.hpp"
#include "Truss/Serializer/Serializers.hpp"
#include "Truss/Utils/SimpleReflection.hpp"
#include "Truss/Material/MaterialBase.hpp"

using namespace Truss;
using namespace std;
namespace
{

    template<typename TBase, typename T> requires std::derived_from<T, TBase>
    std::shared_ptr<TBase> Creator(const TrussDocument& doc)
    {
        auto value = make_shared<T>();
        from_truss(doc, *value);
        return std::static_pointer_cast<TBase>(value);
    }

    SimpleReflection& GetReflection()
    {
        static SimpleReflection refl;
        if (refl.IsEmpty()) [[unlikely]]
        {
            refl.Register("Elastic", Creator<Material::MaterialBase, Material::Elastic>);
            refl.Register("PlaneNodeDisplacement", Creator<Constraint::ConstraintBase, Constraint::PlaneNodeDisplacement>);
            refl.Register("PlaneBar", Creator<Element::ElementBase, Element::PlaneBar>);
            refl.Register("PlaneNodeForce", Creator<Load::LoadBase, Load::PlaneNodeForce>);
            refl.Register("Section_PlaneBar", Creator<Section::SectionBase, Section::Section_PlaneBar>);
        }
        return refl;
    }
}


void TrussSolver::LoadTrussDocument(const TrussDocument &doc)
{
    GetPlaneNodes(doc);
    GetMaterials(doc);
    GetSections(doc);
    GetElements(doc);
    GetConstrains(doc);
    GetLoads(doc);
    BuildAllComponents();
}

VectorX<Numeric> TrussSolver::GetF()
{
    int K_size = GetKSize();
    VectorX<Numeric> F(K_size);
    F.setZero();
    for (int i = 0; i < m_Resources.Loads.size(); ++i)
    {
        auto ids = m_Resources.Loads[i]->GetNodeIds();
        auto load = m_Resources.Loads [i]->GetLoad();
        for (auto id : ids)
        {
            for (int t = 0; t < load.size(); ++t)
            {
                F[id * 2 + t] = load[t];   // TODO: magic number 2
            }
        }
//        int id = m_LoadList[i].PlaneNode->Id;
//        F(id * 2) = m_LoadList[i].XForce;
//        F(id * 2 + 1) = m_LoadList[i].YForce;
    }
    return F;
}

VectorX<Numeric> TrussSolver::GetSimplifiedF()
{
    auto index = GetSimplifiedIndex();
    auto F = GetF();
    return F(index);
}

MatrixX<Numeric> TrussSolver::GetK()
{
    int element_number = (int) m_Resources.Elements.size();
    int K_size = GetKSize();
    MatrixX<Numeric> K(K_size, K_size);
    K.setZero();
    for (int i = 0; i < element_number; i++)
    {
        auto ids = m_Resources.Elements[i]->GetNodeIds();
        int p = 2 * ids[0]; // TODO: correct
        int q = 2 * ids[1];
        auto ke = m_Resources.Elements[i]->GetStiffnessGlobal();
        K.block(p, p, 2, 2) += ke.block(0, 0, 2, 2);
        K.block(p, q, 2, 2) += ke.block(0, 2, 2, 2);
        K.block(q, p, 2, 2) += ke.block(2, 0, 2, 2);
        K.block(q, q, 2, 2) += ke.block(2, 2, 2, 2);
    }
    return K;
}

MatrixX<Numeric> TrussSolver::GetSimplifiedK()
{
    auto index = GetSimplifiedIndex();
    auto K = GetK();
    return K(index, index);
}

int TrussSolver::GetKSize() const noexcept
{
    return GetNumberOfNode() * 2;
}

std::vector<int> TrussSolver::GetSimplifiedIndex()
{
    int K_size = GetKSize();
    std::vector<bool> flag(K_size, true);
    for (int i = 0; i < m_Resources.Constraints.size(); ++i)
    {
        auto ids = m_Resources.Constraints[i]->GetNodeIds();
        auto constraint = m_Resources.Constraints[i]->GetConstraint();
        for (auto id : ids)
        {
            for (int t = 0; t < constraint.size(); ++t)
            {
                flag[id * 2 + t] = !constraint[t];   // TODO: magic number 2
            }
        }
//        flag[id * 2] = !m_ConstraintList[i].XConstraint;
//        flag[id * 2 + 1] = !m_ConstraintList[i].YConstraint;
    }
    std::vector<int> result;
    for (int i = 0; i < K_size; ++i)
    {
        if (flag[i]) result.push_back(i);
    }
    return result;
}

int TrussSolver::GetNumberOfNode() const noexcept
{
    return static_cast<int>(m_Resources.PlaneNodes.size());
}

void TrussSolver::GetPlaneNodes(const TrussDocument& doc)
{
    auto& array = doc["PlaneNode"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto node = array[i].Get<PlaneNode>();
        node.Id = i;
        m_Resources.PlaneNodes.insert({node.Key, node});
    }
}

void TrussSolver::GetMaterials(const TrussDocument &doc)
{
    auto &array = doc["Material"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
//        auto obj = GetReflection()
//                .Invoke<shared_ptr<Material::MaterialBase>, const TrussDocument&>(type, array[i]);
        auto obj = array[i].Get<Material::Elastic>();
        obj.Id = i;
        m_Resources.Materials.insert({ obj.Key, obj });
    }
}

void TrussSolver::GetElements(const TrussDocument &doc)
{
    auto &array = doc["Element"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetReflection()
        .Invoke<shared_ptr<Element::ElementBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Elements.insert({ obj->Key, obj });
    }
}

void TrussSolver::GetConstrains(const TrussDocument &doc)
{
    auto &array = doc["Constraint"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetReflection()
                .Invoke<shared_ptr<Constraint::ConstraintBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Constraints.insert({ obj->Key, obj });
    }
}

void TrussSolver::GetLoads(const TrussDocument &doc)
{
    auto &array = doc["Load"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetReflection()
                .Invoke<shared_ptr<Load::LoadBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Loads.insert({ obj->Key, obj });
    }
}

void TrussSolver::GetSections(const TrussDocument& doc)
{
    auto &array = doc["Section"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
//        auto obj = GetReflection()
//                .Invoke<shared_ptr<Section::SectionBase>, const TrussDocument&>(type, array[i]);
        auto obj = array[i].Get<Section::Section_PlaneBar>();
        obj.Id = i;
        m_Resources.Sections.insert({ obj.Key, obj });
    }
}


void TrussSolver::BuildAllComponents()
{
    // order is important!!
    for(auto&& [_, entity] : m_Resources.Sections) entity.Build(m_Resources);
    for(auto&& [_, entity] : m_Resources.Elements) entity->Build(m_Resources);
    for(auto&& [_, entity] : m_Resources.Constraints) entity->Build(m_Resources);
    for(auto&& [_, entity] : m_Resources.Loads) entity->Build(m_Resources);
}
