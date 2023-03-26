#pragma once
#include <tuple>
namespace Truss
{
    constexpr auto NodesCsvHeader =
            std::make_tuple("node_id", "node_key", "x", "y", "z", "dx", "dy", "dz", "rx", "ry", "rz");

    constexpr auto ElementsCsvHeader =
            std::make_tuple("element_key", "element_name", "node_count",
                            "node1_id", "node2_id", "node3_id", "node4_id", "node5_id", "node6_id", "node7_id", "node8_id",
                            "stress_x", "stress_y", "stress_z", "stress_xy", "stress_yz", "stress_zx");

}// namespace Truss