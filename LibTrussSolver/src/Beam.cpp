#include "Truss/Element/Beam.hpp"

namespace Truss::Element
{

    std::vector<ID> Beam::GetNodeIds() const
    {
        return { this->LeftNode->Id, this->RightNode->Id };
    }
}