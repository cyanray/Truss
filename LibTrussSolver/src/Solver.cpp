#include "Truss/Solver.hpp"
#include "Truss/Serializer/Serializers.hpp"
#include "Truss/Utils/SimpleReflection.hpp"
#include "Truss/Material/MaterialBase.hpp"
#include "Truss/Common/DegreeOfFreedom.hpp"

#include <vector>

using namespace Truss;
using namespace std;

void TrussSolver::LoadTrussDocument(const TrussDocument &doc)
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
        for (auto id : ids)
        {
            F.block<6,1>(id * ALL_DOF, 0) += load;
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
    const static Eigen::Array<int, 6, 1> index_dof{0, 1, 2, 3, 4, 5};
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
        for (int id : ids)
        {
            auto tmp = index_dof + id * ALL_DOF;
            index.insert(index.end(), tmp.begin(), tmp.end());
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
    return GetNumberOfNode() * ALL_DOF;
}

std::vector<int> TrussSolver::GetSimplifiedIndex()
{
    // 初始化：所有节点都拥有 6 个自由度
    int K_size = GetKSize();
    std::vector<bool> flag(K_size, true);
    // 根据 Constraint 限制自由度
    for (int i = 0; i < m_Resources.Constraints.size(); ++i)
    {
        auto ids = m_Resources.Constraints[i]->GetNodeIds();
        auto constraint = m_Resources.Constraints[i]->GetConstraint();
        for (auto id : ids)
        {
            for (int t = 0; t < constraint.size(); ++t)
            {
                flag[id * ALL_DOF + t] = !constraint[t];
            }
        }
    }
    // 根据 Element 获取各个节点的自由度
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
    // 限制不存在的自由度
    for (int i = 0; i < Dof.size(); ++i)
    {
        flag[i * ALL_DOF + 0] = flag[i * ALL_DOF + 0] && static_cast<bool>(Dof[i] & DegreeOfFreedom::X);
        flag[i * ALL_DOF + 1] = flag[i * ALL_DOF + 1] && static_cast<bool>(Dof[i] & DegreeOfFreedom::Y);
        flag[i * ALL_DOF + 2] = flag[i * ALL_DOF + 2] && static_cast<bool>(Dof[i] & DegreeOfFreedom::Z);
        flag[i * ALL_DOF + 3] = flag[i * ALL_DOF + 3] && static_cast<bool>(Dof[i] & DegreeOfFreedom::RX);
        flag[i * ALL_DOF + 4] = flag[i * ALL_DOF + 4] && static_cast<bool>(Dof[i] & DegreeOfFreedom::RY);
        flag[i * ALL_DOF + 5] = flag[i * ALL_DOF + 5] && static_cast<bool>(Dof[i] & DegreeOfFreedom::RZ);
    }
    // 转换为 index
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

void TrussSolver::LoadNodes(const TrussDocument& doc)
{
    auto& array = doc["Node"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto node = array[i].Get<Node>();
        node.Id = i;    // OR: Re-indexing
        m_Resources.Nodes.insert({node.Key, node});
    }
}

void TrussSolver::LoadMaterials(const TrussDocument &doc)
{
    auto &array = doc["Material"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
//        auto obj = GetCompomentReflection()
//                .Invoke<shared_ptr<Material::MaterialBase>, const TrussDocument&>(type, array[i]);
        auto obj = array[i].Get<Material::Elastic>();
        obj.Id = i;
        m_Resources.Materials.insert({ obj.Key, obj });
    }
}

void TrussSolver::LoadElements(const TrussDocument &doc)
{
    auto &array = doc["Element"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
        .Invoke<shared_ptr<Element::ElementBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Elements.insert({ obj->Key, obj });
    }
}

void TrussSolver::LoadConstrains(const TrussDocument &doc)
{
    auto &array = doc["Constraint"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
                .Invoke<shared_ptr<Constraint::ConstraintBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Constraints.insert({ obj->Key, obj });
    }
}

void TrussSolver::LoadLoads(const TrussDocument &doc)
{
    auto &array = doc["Load"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
                .Invoke<shared_ptr<Load::LoadBase>, const TrussDocument&>(type, array[i]);
        obj->Id = i;
        m_Resources.Loads.insert({ obj->Key, obj });
    }
}

void TrussSolver::LoadSections(const TrussDocument& doc)
{
    auto &array = doc["Section"];
    int len = (int) array.Size();
    for (int i = 0; i < len; ++i)
    {
        auto type = array[i]["type"].Get<string>();
        auto obj = GetCompomentReflection()
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
