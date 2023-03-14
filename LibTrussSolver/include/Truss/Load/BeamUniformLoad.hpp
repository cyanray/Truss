#pragma once

#include "LoadBase.hpp"
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Element/Beam.hpp"

namespace Truss::Load
{
    class BeamUniformLoad : public LoadBase
    {
    public:
        Numeric YForce{};
        Numeric ZForce{};
        ID BeamKey{INVALID_ID};
        Element::Beam* Beam{};

        void Build(Resources& resources) override;

        [[nodiscard]] std::vector<ID> GetNodeIds() const override;

        [[nodiscard]] std::vector<std::pair<ID, LoadVector>> GetLoads() const override;
    };
}