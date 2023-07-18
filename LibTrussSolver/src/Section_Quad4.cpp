#include "Truss/Section/Section_Quad4.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Section
{
    void Section_Quad4::Build(Resources& resources)
    {
        Mat = &resources.Materials.at(MaterialKey);
    }
}