#include "Truss/Load/BeamUniformLoad.hpp"

namespace Truss::Load
{

    void BeamUniformLoad::Build(Resources& resources)
    {
        auto& beam = resources.Elements.at(BeamKey);
        Beam = static_pointer_cast<Element::Beam>(beam).get();

    }

    std::vector<ID> BeamUniformLoad::GetNodeIds() const
    {
        return {Beam->LeftNode->Id, Beam->RightNode->Id};
    }

    std::vector<std::pair<ID, LoadVector>> BeamUniformLoad::GetLoads() const
    {
        Numeric L = Beam->GetBeamLength();
        Numeric L2 = L * L;
        Numeric  k_force = static_cast<Numeric>(0.5) * L;
        Numeric  k_moment = static_cast<Numeric>(1.0 / 12.0) * L2;
        Numeric yforce = k_force * YForce;
        Numeric zmoment = k_moment * YForce;
        Numeric zforce = k_force * ZForce;
        Numeric ymoment = k_moment * ZForce;
        LoadVector left_load = {0, yforce, zforce, 0, ymoment, zmoment};
        LoadVector right_load = {0, yforce, zforce, 0, -ymoment, -zmoment};
        return {std::make_pair(Beam->LeftNode->Id, left_load),
                std::make_pair(Beam->RightNode->Id, right_load)};
    }

}// namespace Truss::Load