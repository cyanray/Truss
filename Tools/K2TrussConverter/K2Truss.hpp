#pragma once
#include <cassert>
#include <string>

#include <TrussDocument/TrussDocument.hpp>

#include "KDoc.hpp"
using std::string;

namespace Truss::Tools
{

    TrussDocument GetNodes(const KDoc& kdoc)
    {
        // nid, x, y, z, tc, rc
        TrussDocument result = TrussDocument::Array();
        for (auto& row: kdoc.Data)
        {
            TrussDocument node = TrussDocument::Object();
            node["key"] = int(row[0]);
            node["x"] = row[1];
            node["y"] = row[2];
            node["z"] = row[3];
            result.Add(std::move(node));
        }
        return result;
    }

    TrussDocument GetTriangles(const KDoc& kdoc)
    {
        // eid, pid,  n1,  n2,  n3,  n4,  n5,  n6,  n7,  n8
        TrussDocument result = TrussDocument::Array();
        for (auto& row: kdoc.Data)
        {
            if (row[4] != row[5]) continue;
            TrussDocument triangle = TrussDocument::Object();
            triangle["type"] = "CSTriangle";
            triangle["key"] = int(row[0]);
            triangle["node1_key"] = int(row[2]);
            triangle["node2_key"] = int(row[3]);
            triangle["node3_key"] = int(row[4]);
            triangle["section_key"] = 0;
            result.Add(std::move(triangle));
        }
        return result;
    }

}// namespace Truss::Tools
