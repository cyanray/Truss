from Common import Element, Node
import csv
import numpy as np


# read csv file and return dict of np.array
def read_csv(filename):
    with open(filename, 'r') as file:
        reader = csv.DictReader(file)
        result = {}
        for row in reader:
            for column, value in row.items():
                result.setdefault(column, []).append(value)
    for key in result:
        result[key] = np.array(result[key])
    return result


def read_nodes(filename: str) -> dict:
    node_dict = read_csv(filename)
    result = {}
    for i in range(len(node_dict['node_key'])):
        node = Node.Node()
        node.Key = int(node_dict['node_key'][i])
        node.Id = int(node_dict['node_id'][i])
        node.X = float(node_dict['x'][i])
        node.Y = float(node_dict['y'][i])
        node.Z = float(node_dict['z'][i])
        node.DX = float(node_dict['dx'][i])
        node.DY = float(node_dict['dy'][i])
        node.DZ = float(node_dict['dz'][i])
        result[node.Id] = node
    return result


def read_elements(filename):
    elem_dict = read_csv(filename)
    result = {}
    for i in range(len(elem_dict['element_key'])):
        element = Element.Element()
        element.Key = int(elem_dict['element_key'][i])
        element.Id = int(elem_dict['element_id'][i])
        element.Name = elem_dict['element_name'][i]
        element.NodeCount = int(elem_dict['node_count'][i])
        element.Node1Id = int(elem_dict['node1_id'][i])
        element.Node2Id = int(elem_dict['node2_id'][i])
        element.Node3Id = int(elem_dict['node3_id'][i])
        element.Node4Id = int(elem_dict['node4_id'][i])
        element.Node5Id = int(elem_dict['node5_id'][i])
        element.Node6Id = int(elem_dict['node6_id'][i])
        element.Node7Id = int(elem_dict['node7_id'][i])
        element.Node8Id = int(elem_dict['node8_id'][i])
        element.StressX = float(elem_dict['stress_x'][i])
        element.StressY = float(elem_dict['stress_y'][i])
        element.StressZ = float(elem_dict['stress_z'][i])
        element.StressXY = float(elem_dict['stress_xy'][i])
        element.StressYZ = float(elem_dict['stress_yz'][i])
        element.StressZX = float(elem_dict['stress_zx'][i])
        result[element.Id] = element
    return result
