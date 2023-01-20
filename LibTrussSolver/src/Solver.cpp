#include "Truss/Solver.hpp"
using namespace Truss;

void TrussSolver::LoadTrussDocument(const TrussDocument &doc) {
    GetPlaneNodes(doc);
    GetMaterials(doc);
    GetElements(doc);
    GetConstrains(doc);
    GetLoads(doc);
}

VectorX<Numeric> TrussSolver::GetF() {
    int K_size = GetKSize();
    VectorX<Numeric> F(K_size);
    F.setZero();
    for (int i = 0; i < m_LoadList.size(); ++i)
    {
        int id = m_LoadList[i].PlaneNode->Id;
        F(id * 2) = m_LoadList[i].XForce;
        F(id * 2 + 1) = m_LoadList[i].YForce;
    }
    return F;
}

VectorX<Numeric> TrussSolver::GetSimplifiedF() {
    auto index = GetSimplifiedIndex();
    auto F = GetF();
    return F(index);
}

MatrixX<Numeric> TrussSolver::GetK() {
    int element_number = (int)m_ElementList.size();
    int K_size = GetKSize();
    MatrixX<Numeric> K(K_size, K_size);
    K.setZero();
    for (int i = 0; i < element_number; i++)
    {
        int p = 2 * m_ElementList[i].LeftNode->Id;
        int q = 2 * m_ElementList[i].RightNode->Id;
        auto ke = m_ElementList[i].GetStiffnessGlobal();
        K.block(p, p, 2, 2) += ke.block(0, 0, 2, 2);
        K.block(p, q, 2, 2) += ke.block(0, 2, 2, 2);
        K.block(q, p, 2, 2) += ke.block(2, 0, 2, 2);
        K.block(q, q, 2, 2) += ke.block(2, 2, 2, 2);
    }
    return K;
}

MatrixX<Numeric> TrussSolver::GetSimplifiedK() {
    auto index = GetSimplifiedIndex();
    auto K = GetK();
    return K(index, index);
}

int TrussSolver::GetKSize() const noexcept {
    return GetNumberOfNode() * 2;
}

std::vector<int> TrussSolver::GetSimplifiedIndex() {
    int K_size = GetKSize();
    std::vector<bool> flag(K_size, true);
    for (int i = 0; i < m_ConstraintList.size(); ++i)
    {
        int id = m_ConstraintList[i].PlaneNode->Id;
        flag[id * 2] = !m_ConstraintList[i].XConstraint;
        flag[id * 2 + 1] = !m_ConstraintList[i].YConstraint;
    }
    std::vector<int> result;
    for (int i = 0; i < K_size; ++i)
    {
        if (flag[i]) result.push_back(i);
    }
    return result;
}

int TrussSolver::GetNumberOfNode() const noexcept {
    return static_cast<int>(m_NodeList.size());
}

void TrussSolver::GetPlaneNodes(const TrussDocument &doc) {
    auto& planeNodeArray = doc["PlaneNode"];
    int len = (int)planeNodeArray.size();
    for (int i = 0; i < len; ++i)
    {
        int key = planeNodeArray[i]["key"].get<int>();
        Numeric x = planeNodeArray[i]["x"].get<Numeric>();
        Numeric y = planeNodeArray[i]["y"].get<Numeric>();
        m_NodeList.insert({key, PlaneNode{ .Id = i, .X = x, .Y = y  } });
    }
}

void TrussSolver::GetMaterials(const TrussDocument &doc) {
    auto& materialArray = doc["Material"];
    int len = (int)materialArray.size();
    for (int i = 0; i < len; ++i)
    {
        int key = materialArray[i]["key"].get<int>();
        auto type = materialArray[i]["type"].get<string>();
        auto name = materialArray[i]["name"].get<string>();
        Numeric rho = materialArray[i]["rho"].get<Numeric>();
        Numeric E = materialArray[i]["E"].get<Numeric>();
        Numeric pr = materialArray[i]["pr"].get<Numeric>();
        m_MaterialList.insert({key, Material::Elastic
                {
                        .Id = i,
                        .Density = rho,
                        .YoungsModules = E,
                        .PoissonRation = pr
                } });
    }
}

void TrussSolver::GetElements(const TrussDocument &doc) {
    auto& elementArray = doc["Element"];
    int len = (int)elementArray.size();
    for (int i = 0; i < len; ++i)
    {
        int key = elementArray[i]["key"].get<int>();
        auto type = elementArray[i]["type"].get<string>();
        // auto name = elementArray[i]["name"].get<string>();
        int node1_key = elementArray[i]["node1_key"].get<int>();
        int node2_key = elementArray[i]["node2_key"].get<int>();
        int mat_key = elementArray[i]["mat_key"].get<int>();
        Numeric area = elementArray[i]["area"].get<Numeric>();
        m_ElementList.insert({ key, Element::Bar
                {
                        .Id = i,
                        .Area = area,
                        .Mat = &m_MaterialList[mat_key],
                        .LeftNode = &m_NodeList[node1_key],
                        .RightNode = &m_NodeList[node2_key]
                } });
    }
}

void TrussSolver::GetConstrains(const TrussDocument &doc) {
    auto& constraintArray = doc["Constraint"];
    int len = (int)constraintArray.size();
    for (int i = 0; i < len; ++i)
    {
        int key = constraintArray[i]["key"].get<int>();
        auto type = constraintArray[i]["type"].get<string>();
        // auto name = constraintArray[i]["name"].get<string>();
        int node_key = constraintArray[i]["node_key"].get<int>();
        bool x = constraintArray[i]["x"].get<bool>();
        bool y = constraintArray[i]["y"].get<bool>();
        m_ConstraintList.insert({ key, Constraint::PlaneNodeDisplacement
                {
                        .Id = i,
                        .XConstraint = x,
                        .YConstraint = y,
                        .PlaneNode = &m_NodeList[node_key]
                } });
    }
}

void TrussSolver::GetLoads(const TrussDocument &doc) {
    auto& loadArray = doc["Load"];
    int len = (int)loadArray.size();
    for (int i = 0; i < len; ++i)
    {
        int key = loadArray[i]["key"].get<int>();
        auto type = loadArray[i]["type"].get<string>();
        // auto name = loadArray[i]["name"].get<string>();
        int node_key = loadArray[i]["node_key"].get<int>();
        Numeric xForce = loadArray[i]["XForce"].get<Numeric>();
        Numeric yForce = loadArray[i]["YForce"].get<Numeric>();
        m_LoadList.insert({ key, Load::PlaneNodeForce
                {
                        .Id = i,
                        .XForce = xForce,
                        .YForce = yForce,
                        .PlaneNode = &m_NodeList[node_key]
                } });
    }
}
