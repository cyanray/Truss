#include "Truss/Section/Section_PlaneBar.hpp"
#include "Truss/Common/Resources.hpp"

using namespace Truss;
using namespace std;

void Section::Section_PlaneBar::Build(Resources& resources)
{
    Mat = &resources.Materials.at(MaterialKey);
}
