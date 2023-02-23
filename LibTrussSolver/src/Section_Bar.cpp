#include "Truss/Section/Section_Bar.hpp"
#include "Truss/Common/Resources.hpp"

using namespace Truss;
using namespace std;

void Section::Section_Bar::Build(Resources& resources)
{
    Mat = &resources.Materials.at(MaterialKey);
}
