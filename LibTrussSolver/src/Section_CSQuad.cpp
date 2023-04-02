#include "Truss/Section/Section_CSQuad.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Section
{
    void Section_CSQuad::Build(Resources& resources)
    {
        Mat = &resources.Materials.at(MaterialKey);
    }
}