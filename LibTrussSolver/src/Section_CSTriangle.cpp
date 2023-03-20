#include "Truss/Section/Section_CSTriangle.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Section
{
    void Section_CSTriangle::Build(Resources& resources)
    {
        Mat = &resources.Materials.at(MaterialKey);
    }
}
