#include "K2Truss.hpp"
using namespace Truss;

namespace Truss::Tools
{
    TrussDocument GetNodes(const KDoc& kdoc)
    {
        // nid, x, y, z, tc, rc
        TrussDocument result = TrussDocument::Array();
        for (auto& row: kdoc.Data)
        {
            TrussDocument node = TrussDocument::Object();
            node["key"] = int(lround(row[0]));
            node["x"] = row[1];
            node["y"] = row[2];
            node["z"] = row[3];
            result.Add(std::move(node));
        }
        return result;
    }

    TrussDocument GetPlane(const KDoc& kdoc)
    {
        // eid, pid,  n1,  n2,  n3,  n4,  n5,  n6,  n7,  n8
        TrussDocument result = TrussDocument::Array();
        for (auto& row: kdoc.Data)
        {
            bool isQuad = (row[4] != row[5]);
            TrussDocument plane = TrussDocument::Object();
            plane["type"] = (isQuad) ? "Quad4" : "CSTriangle";
            plane["key"] = int(lround(row[0]));
            plane["section_key"] = 0;
            plane["node1_key"] = int(lround(row[2]));
            plane["node2_key"] = int(lround(row[3]));
            plane["node3_key"] = int(lround(row[4]));
            if (isQuad) plane["node4_key"] = int(lround(row[5]));
            result.Add(std::move(plane));
        }
        return result;
    }
    TrussDocument GetNodeSet(const KDoc& kdoc)
    {
        TrussDocument result = TrussDocument::Object();
        result["key"] = 0;
        result["type"] = "NodeSet";
        result["nodes"] = TrussDocument::Array();
        for (int i = 1; i < kdoc.Data.size(); ++i)
        {
            for (auto val: kdoc.Data[i])
            {
                if (val == 0) continue;
                result["nodes"].Add(int(lround(val)));
            }
        }
        return result;
    }

}// namespace Truss::Tools