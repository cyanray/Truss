#include "Truss/Solver.hpp"
#include "Truss/Common/DegreeOfFreedom.hpp"
#include "Truss/Material/MaterialBase.hpp"
#include "Truss/Serializer/Serializers.hpp"
#include "Truss/Utils/SimpleReflection.hpp"

#include <bitset>
#include <vector>

using namespace Truss;
using namespace std;

void TrussSolver::LoadTrussDocument(const TrussDocument& doc)
{
    LoadNodes(doc);
    LoadMaterials(doc);
    LoadSections(doc);
    LoadElements(doc);
    LoadConstrains(doc);
    LoadLoads(doc);
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
        for (auto id: ids)
        {
            F.block<6, 1>(id * ALL_DOF, 0) += load;
        }
    }
    return F;
}

VectorX<Numeric> TrussSolver::GetSimplifiedF()
{
    auto index = GetFreedomIndex();
    auto F = GetF();
    return F(index);
}

MatrixX<Numeric> TrussSolver::GetK()
{
    int element_count = (int) m_Resources.Elements.size();
    int K_size = GetKSize();
    MatrixX<Numeric> K = MatrixX<Numeric>::Zero(K_size, K_size);
    for (int i = 0; i < element_count; ++i)
    {
        auto ids = m_Resources.Elements[i]->GetNodeIds();
        auto ke = m_Resources.Elements[i]->GetStiffnessGlobal();
        // Get index of element node in K
        std::vector<int> index;
        index.reserve(ids.size() * ALL_DOF);
        for (int id: ids)
        {
            auto tmp = DOF_INDEX + id * ALL_DOF;
            index.insert(index.end(), tmp.begin(), tmp.end());
        }
        K(index, index) += ke;
    }
    return K;
}

MatrixX<Numeric> TrussSolver::GetSimplifiedK()
{
    auto index = GetFreedomIndex();
    auto K = GetK();
    return K(index, index);
}

int TrussSolver::GetKSize() const noexcept
{
    return GetNumberOfNode() * ALL_DOF;
}

std::vector<int> TrussSolver::GetFreedomIndex()
{
    // 初始化，所有节点都没有自由度
    int node_count = GetNumberOfNode();
    vector<DegreeOfFreedom> Dof(node_count);
    // 根据 element 赋予自由度
    for (auto& [_, element]: m_Resources.Elements)
    {
        auto ids = element->GetNodeIds();
        auto dof = element->GetNodeDegreeOfFreedom();
        for (int id: ids) Dof[id] |= dof;
    }
    // 根据 constraint 限制自由度
    for (auto& [_, constraint]: m_Resources.Constraints)
    {
        auto ids = constraint->GetNodeIds();
        auto dof = constraint->GetDegreeOfFreedom();
        for (auto id: ids) Dof[id] &= dof;
    }
    // 转换为 index
    vector<int> index;
    for (int i = 0; i < node_count; ++i)
    {
        std::bitset<ALL_DOF> dof_bit = (char) Dof[i];
        if (dof_bit.none()) continue;
        if (dof_bit.all())
        {
            auto tmp = DOF_INDEX + i * ALL_DOF;
            index.insert(index.end(), tmp.begin(), tmp.end());
        }
        for (int j = 0; j < ALL_DOF; ++j)
        {
            if (dof_bit[j]) index.push_back(i * ALL_DOF + j);
        }
    }
    return index;
}

int TrussSolver::GetNumberOfNode() const noexcept
{
    return static_cast<int>(m_Resources.Nodes.size());
}

void TrussSolver::LoadNodes(const TrussDocument& doc)
{
    auto& array = doc["Node"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto node = array[i].Get<Node>();
        node.Id = i;// OR: Re-indexing
        m_Resources.Nodes.insert({node.Key, node});
    }
}

void TrussSolver::LoadMaterials(const TrussDocument& doc)
{
    auto& array = doc["Material"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        //        auto obj = GetCompomentReflection()
        //                .Invoke<shared_ptr<Material::MaterialBase>, const TrussDocument&>(type, array[i]);
        auto obj = array[i].Get<Material::Elastic>();
        obj.Id = i;
        m_Resources.Materials.insert({obj.Key, obj});
    }
}

void TrussSolver::LoadElements(const TrussDocument& doc)
{
    auto& array = doc["Element"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
                           .Invoke<shared_ptr<Element::ElementBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Elements.insert({obj->Key, obj});
    }
}

void TrussSolver::LoadConstrains(const TrussDocument& doc)
{
    auto& array = doc["Constraint"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
                           .Invoke<shared_ptr<Constraint::ConstraintBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Constraints.insert({obj->Key, obj});
    }
}

void TrussSolver::LoadLoads(const TrussDocument& doc)
{
    auto& array = doc["Load"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
                           .Invoke<shared_ptr<Load::LoadBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Loads.insert({obj->Key, obj});
    }
}

void TrussSolver::LoadSections(const TrussDocument& doc)
{
    auto& array = doc["Section"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
                           .Invoke<shared_ptr<Section::SectionBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Sections.insert({obj->Key, obj});
    }
}

void TrussSolver::BuildAllComponents()
{
    // order is important!!
    for (auto&& [_, entity]: m_Resources.Sections) entity->Build(m_Resources);
    for (auto&& [_, entity]: m_Resources.Elements) entity->Build(m_Resources);
    for (auto&& [_, entity]: m_Resources.Constraints) entity->Build(m_Resources);
    for (auto&& [_, entity]: m_Resources.Loads) entity->Build(m_Resources);
}
