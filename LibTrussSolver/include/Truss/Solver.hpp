#pragma once
#include <map>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "Truss/Material/Material_Elastic.hpp"
#include "Truss/Element/Element_Bar.hpp"
#include "Truss/Constraint/Constraint_PlaneNodeDisplacement.hpp"
#include "Truss/Load/Load_PlaneNodeForce.hpp"
#include "TrussDocument/TrussDocument.hpp"


namespace Truss
{
    template<typename T = NumericType>
    class TrussSolver
    {
    public:
        void LoadTrussDocument(const TrussDocument& doc)
        {
            getPlaneNodes(doc);
            getMaterials(doc);
            getElements(doc);
            getConstrains(doc);
            getLoads(doc);
        }

        VectorX<T> GetF()
        {
            int K_size = GetKSize();
            VectorX<T> F(K_size);
            F.setZero();
            for (int i = 0; i < m_LoadList.size(); ++i)
            {
                int id = m_LoadList[i].PlaneNode->Id;
                F(id * 2) = m_LoadList[i].XForce;
                F(id * 2 + 1) = m_LoadList[i].YForce;
            }
            return F;
        }

        VectorX<T> GetFSimplified()
        {
            auto index = GetSimplifedIndex();
            auto F = GetF();
            return F(index);
        }

        MatrixX<T> GetK()
        {
            int element_number = (int)m_ElementList.size();
            int K_size = GetKSize();
            MatrixX<T> K(K_size, K_size);
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

        MatrixX<T> GetKSimplified()
        {
            auto index = GetSimplifedIndex();
            auto K = GetK();
            return K(index, index);
        }

        int GetKSize() const noexcept
        {
            return getNumberOfNode() * 2;
        }

        std::vector<int> GetSimplifedIndex()
        {
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

        private:
        std::map<int, PlaneNode<T>> m_NodeList;
        std::map<int, Material::Material_Elastic<T>> m_MaterialList;
        std::map<int, Element::Element_Bar<T>> m_ElementList;
        std::map<int, Constraint::Constraint_PlaneNodeDisplacement<T>> m_ConstraintList;
        std::map<int, Load::Load_PlaneNodeForce<T>> m_LoadList;

        int getNumberOfNode() const noexcept
        {
            return static_cast<int>(m_NodeList.size());
        }

        void getPlaneNodes(const TrussDocument &doc)
        {
            auto& planeNodeArray = doc["PlaneNode"];
            int len = (int)planeNodeArray.size();
            for (int i = 0; i < len; ++i)
            {
                int key = planeNodeArray[i]["key"].get<int>();
                T x = planeNodeArray[i]["x"].get<T>();
                T y = planeNodeArray[i]["y"].get<T>();
                m_NodeList.insert({key, PlaneNode<T>{ .Id = i, .X = x, .Y = y  } });
            }
        }

        void getMaterials(const TrussDocument &doc)
        {
            auto& materialArray = doc["Material"];
            int len = (int)materialArray.size();
            for (int i = 0; i < len; ++i)
            {
                int key = materialArray[i]["key"].get<int>();
                auto type = materialArray[i]["type"].get<string>();
                auto name = materialArray[i]["name"].get<string>();
                T rho = materialArray[i]["rho"].get<T>();
                T E = materialArray[i]["E"].get<T>();
                T pr = materialArray[i]["pr"].get<T>();
                m_MaterialList.insert({key, Material::Material_Elastic<T>
                        {
                            .Id = i,
                            .Density = rho,
                            .YoungsModules = E,
                            .PoissonRation = pr
                        } });
            }
        }

        void getElements(const TrussDocument &doc)
        {
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
                T area = elementArray[i]["area"].get<T>();
                m_ElementList.insert({ key, Element::Element_Bar<T>
                                            {
                                                .Id = i,
                                                .Area = area,
                                                .Mat = &m_MaterialList[mat_key],
                                                .LeftNode = &m_NodeList[node1_key],
                                                .RightNode = &m_NodeList[node2_key]
                                            } });
            }
        }

        void getConstrains(const TrussDocument &doc)
        {
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
                m_ConstraintList.insert({ key, Constraint::Constraint_PlaneNodeDisplacement<T>
                                               {
                                                   .Id = i,
                                                   .XConstraint = x,
                                                   .YConstraint = y,
                                                   .PlaneNode = &m_NodeList[node_key]
                                               } });
            }
        }

        void getLoads(const TrussDocument &doc)
        {
            auto& loadArray = doc["Load"];
            int len = (int)loadArray.size();
            for (int i = 0; i < len; ++i)
            {
                int key = loadArray[i]["key"].get<int>();
                auto type = loadArray[i]["type"].get<string>();
                // auto name = loadArray[i]["name"].get<string>();
                int node_key = loadArray[i]["node_key"].get<int>();
                T xForce = loadArray[i]["XForce"].get<T>();
                T yForce = loadArray[i]["YForce"].get<T>();
                m_LoadList.insert({ key, Load::Load_PlaneNodeForce<T>
                                         {
                                             .Id = i,
                                             .XForce = xForce,
                                             .YForce = yForce,
                                             .PlaneNode = &m_NodeList[node_key]
                                         } });
            }
        }

    };

}