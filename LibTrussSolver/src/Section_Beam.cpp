#include "Truss/Section/Section_Beam.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Section
{

    void Section_Beam::Build(Resources& resources)
    {
        Mat = &resources.Materials.at(MaterialKey);
    }
}
