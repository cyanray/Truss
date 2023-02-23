#include "Truss/Solver.hpp"
#include "Truss/Serializer/Serializers.hpp"
#include "Truss/Utils/SimpleReflection.hpp"
#include "Truss/Material/MaterialBase.hpp"
#include "Truss/Common/DegreeOfFreedom.hpp"

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
            refl.Register("NodeDisplacement", Creator<Constraint::ConstraintBase, Constraint::NodeDisplacement>);
            refl.Register("Bar", Creator<Element::ElementBase, Element::Bar>);
            refl.Register("NodeForce", Creator<Load::LoadBase, Load::NodeForce>);
            refl.Register("Section_Bar", Creator<Section::SectionBase, Section::Section_Bar>);
        }
        return refl;
    }
}

void TrussSolver::LoadTrussDocument(const TrussDocument &doc)
{
    GetNodes(doc);
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
    VectorX<Numeric> F = VectorX<Numeric>::Zero(K_size);
    for (int i = 0; i < m_Resources.Loads.size(); ++i)
    {
        auto ids = m_Resources.Loads[i]->GetNodeIds();
        auto load = m_Resources.Loads[i]->GetLoad();
        for (auto id : ids)
        {
            F.block<6,1>(id * MAX_DOF, 0) += load;
        }
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
    MatrixX<Numeric> K = MatrixX<Numeric>::Zero(K_size, K_size);
    for (int i = 0; i < element_number; ++i)
    {
        auto ids = m_Resources.Elements[i]->GetNodeIds();
        auto ke = m_Resources.Elements[i]->GetStiffnessGlobal();
        VectorX<Numeric> index = VectorX<Numeric>::Zero(ids.size() * MAX_DOF);
        int m = 0;
        for (int id : ids)
        {
            for (size_t t = 0; t < MAX_DOF; ++t)
            {
                index(m++) = id * MAX_DOF + t;
            }
        }
        K(index, index) += ke;
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
    return GetNumberOfNode() * MAX_DOF;
}

std::vector<int> TrussSolver::GetSimplifiedIndex()
{
    // ��ʼ�������нڵ㶼ӵ�� 6 �����ɶ�
    int K_size = GetKSize();
    std::vector<bool> flag(K_size, true);
    // ���� Constraint �������ɶ�
    for (int i = 0; i < m_Resources.Constraints.size(); ++i)
    {
        auto ids = m_Resources.Constraints[i]->GetNodeIds();
        auto constraint = m_Resources.Constraints[i]->GetConstraint();
        for (auto id : ids)
        {
            for (int t = 0; t < constraint.size(); ++t)
            {
                flag[id * MAX_DOF + t] = !constraint[t];
            }
        }
    }
    // ���� Element ��ȡ�����ڵ�����ɶ�
    vector<DegreeOfFreedom> Dof(GetNumberOfNode());
    for (int i = 0; i < m_Resources.Elements.size(); i++)
    {
        auto element = m_Resources.Elements[i];
        auto ids = element->GetNodeIds();
        for (int id : ids)
        {
            Dof[id] |= element->GetNodeDegreeOfFreedom();
        }
    }
    // ���Ʋ����ڵ����ɶ�
    for (int i = 0; i < Dof.size(); ++i)
    {
        flag[i * MAX_DOF + 0] = flag[i * MAX_DOF + 0] && static_cast<bool>(Dof[i] & DegreeOfFreedom::X);
        flag[i * MAX_DOF + 1] = flag[i * MAX_DOF + 1] && static_cast<bool>(Dof[i] & DegreeOfFreedom::Y);
        flag[i * MAX_DOF + 2] = flag[i * MAX_DOF + 2] && static_cast<bool>(Dof[i] & DegreeOfFreedom::Z);
        flag[i * MAX_DOF + 3] = flag[i * MAX_DOF + 3] && static_cast<bool>(Dof[i] & DegreeOfFreedom::RX);
        flag[i * MAX_DOF + 4] = flag[i * MAX_DOF + 4] && static_cast<bool>(Dof[i] & DegreeOfFreedom::RY);
        flag[i * MAX_DOF + 5] = flag[i * MAX_DOF + 5] && static_cast<bool>(Dof[i] & DegreeOfFreedom::RZ);
    }
    // ת��Ϊ index
    std::vector<int> result;
    for (int i = 0; i < K_size; ++i)
    {
        if (flag[i]) result.push_back(i);
    }
    return result;
}

int TrussSolver::GetNumberOfNode() const noexcept
{
    return static_cast<int>(m_Resources.Nodes.size());
}

void TrussSolver::GetNodes(const TrussDocument& doc)
{
    auto& array = doc["Node"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto node = array[i].Get<Node>();
        node.Id = i;
        m_Resources.Nodes.insert({node.Key, node});
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
        auto obj = GetReflection()
                 .Invoke<shared_ptr<Section::SectionBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Sections.insert({ obj->Key, obj });
    }
}

void TrussSolver::BuildAllComponents()
{
    // order is important!!
    for(auto&& [_, entity] : m_Resources.Sections) entity->Build(m_Resources);
    for(auto&& [_, entity] : m_Resources.Elements) entity->Build(m_Resources);
    for(auto&& [_, entity] : m_Resources.Constraints) entity->Build(m_Resources);
    for(auto&& [_, entity] : m_Resources.Loads) entity->Build(m_Resources);
}
