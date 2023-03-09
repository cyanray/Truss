#include "Truss/Solver.hpp"
#include "Truss/Common/DegreeOfFreedom.hpp"
#include "Truss/Material/MaterialBase.hpp"
#include "Truss/Serializer/Serializers.hpp"
#include "Truss/Utils/SimpleReflection.hpp"

#include <bitset>
#include <ranges>
#include <vector>

using namespace Truss;
using namespace std;


namespace
{
    template<typename T, typename TContainer>
    void LoadCompoments(std::string_view name, const TrussDocument& doc, TContainer&& container)
    {
        vector<T> result;
        auto& array = doc[name];
        int len = (int) array.Count();
        for (int i = 0; i < len; ++i)
        {
            auto obj = array[i].Get<T>();
            obj.Id = i;
            container.emplace(obj.Key, obj);
        }
    }

    template<typename T, typename TContainer>
    void LoadCompomentsWeak(std::string_view name, const TrussDocument& doc, TContainer&& container)
    {
        auto& array = doc[name];
        int len = (int) array.Count();
        for (int i = 0; i < len; ++i)
        {
            auto type = array[i]["type"].Get<string>();
            auto obj = GetCompomentReflection()
                               .Invoke<shared_ptr<T>, const TrussDocument&>(type, array[i]);
            obj->Id = i;
            container.emplace(obj->Key, obj);
        }
    }
}// namespace

void TrussSolver::LoadTrussDocument(const TrussDocument& doc)
{
    LoadCompoments<Node>("Node", doc, m_Resources.Nodes);
    LoadCompoments<Material::Elastic>("Material", doc, m_Resources.Materials);
    LoadCompomentsWeak<Element::ElementBase>("Element", doc, m_Resources.Elements);
    LoadCompomentsWeak<Constraint::ConstraintBase>("Constraint", doc, m_Resources.Constraints);
    LoadCompomentsWeak<Load::LoadBase>("Load", doc, m_Resources.Loads);
    LoadCompomentsWeak<Section::SectionBase>("Section", doc, m_Resources.Sections);

    BuildAllComponents();
}

void TrussSolver::BuildAllComponents()
{
    // order is important!!
    for (auto&& [_, entity]: m_Resources.Sections) entity->Build(m_Resources);
    for (auto&& [_, entity]: m_Resources.Elements) entity->Build(m_Resources);
    for (auto&& [_, entity]: m_Resources.Constraints) entity->Build(m_Resources);
    for (auto&& [_, entity]: m_Resources.Loads) entity->Build(m_Resources);
}

MatrixX<Numeric> TrussSolver::GetK()
{
    int K_size = GetKSize();
    MatrixX<Numeric> K = MatrixX<Numeric>::Zero(K_size, K_size);
    for (auto& [_, element]: m_Resources.Elements)
    {
        auto ids = element->GetNodeIds();
        auto ke = element->GetStiffnessGlobal();
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

VectorX<Numeric> TrussSolver::GetF()
{
    int K_size = GetKSize();
    VectorX<Numeric> F = VectorX<Numeric>::Zero(K_size);
    for (auto&& [_, load_item]: m_Resources.Loads)
    {
        auto ids = load_item->GetNodeIds();
        auto load = load_item->GetLoad();
        for (auto id: ids)
        {
            F.block<6, 1>(id * ALL_DOF, 0) += load;
        }
    }
    return F;
}


MatrixX<Numeric> TrussSolver::GetSimplifiedK()
{
    auto index = GetFreedomIndex();
    auto K = GetK();
    return K(index, index);
}

VectorX<Numeric> TrussSolver::GetSimplifiedF()
{
    auto index = GetFreedomIndex();
    auto F = GetF();
    return F(index);
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
