#pragma once
#include <cassert>
#include <string>

#include <TrussDocument/TrussDocument.hpp>

#include "KDoc.hpp"
using std::string;

namespace Truss::Tools
{

    TrussDocument GetNodes(const KDoc& kdoc);

    TrussDocument GetPlane(const KDoc& kdoc);

    TrussDocument GetNodeSet(const KDoc& kdoc);

}// namespace Truss::Tools
