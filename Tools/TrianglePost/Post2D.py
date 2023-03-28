import os
import csv
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.collections import PatchCollection
from matplotlib.patches import Polygon

if not os.path.exists('figures'):
    os.makedirs('figures')


def is_float(string):
    try:
        float(string)
        return True
    except ValueError:
        return False


def read_csv(filename):
    with open(filename, 'r') as file:
        reader = csv.DictReader(file)
        result = {}
        for row in reader:
            for column, value in row.items():
                result.setdefault(column, []).append(value)
    for key in result:
        result[key] = np.array(result[key])
        if is_float(result[key][0]):
            result[key] = result[key].astype(float)
    return result


def von_mises_stress(x, y, xy):
    return np.sqrt(x ** 2 - x * y + y ** 2 + 3 * xy ** 2)


class PostProcessing:
    def __init__(self, elements_csv_path, nodes_csv_path):
        self.elements = read_csv(elements_csv_path)
        self.nodes = read_csv(nodes_csv_path)
        self.node_id_to_index_map = {value: index for index, value in enumerate(self.nodes['node_id'])}

    def get_element_node_index3(self, element_index):
        node1_id = self.elements['node1_id'][element_index]
        node2_id = self.elements['node2_id'][element_index]
        node3_id = self.elements['node3_id'][element_index]
        node1_index = self.node_id_to_index_map[node1_id]
        node2_index = self.node_id_to_index_map[node2_id]
        node3_index = self.node_id_to_index_map[node3_id]
        return [node1_index, node2_index, node3_index]

    def get_node_xy(self, node_index_list):
        x = self.nodes['x'][node_index_list]
        y = self.nodes['y'][node_index_list]
        return np.array([x, y]).T

    def get_node_displacement_xy(self, node_index_list):
        dx = self.nodes['dx'][node_index_list]
        dy = self.nodes['dy'][node_index_list]
        return np.array([dx, dy]).T

    def get_element_stress(self, element_index):
        stress_x = self.elements['stress_x'][element_index]
        stress_y = self.elements['stress_y'][element_index]
        stress_xy = self.elements['stress_xy'][element_index]
        return von_mises_stress(stress_x, stress_y, stress_xy)

    def draw_fem_mesh(self):
        fig, ax = plt.subplots()
        patches = []
        for i in range(len(self.elements['element_key'])):
            element_name = self.elements['element_name'][i]
            if element_name == 'CSTriangle':
                node_index_list = self.get_element_node_index3(i)
                points = self.get_node_xy(node_index_list)
                polygon = Polygon(points, closed=True)
                patches.append(polygon)
        p = PatchCollection(patches, linewidths=0.5, edgecolor='k', alpha=0.7)
        ax.add_collection(p)
        plt.title("Mesh")
        plt.xlabel("x")
        plt.ylabel("y")
        plt.axis('square')
        plt.savefig("figures/mesh_figure.pdf", format="pdf", bbox_inches="tight")
        plt.show()

    def draw_displacement(self, scale=1.0):
        fig, ax = plt.subplots()
        patches = []
        for i in range(len(self.elements['element_key'])):
            element_name = self.elements['element_name'][i]
            if element_name == 'CSTriangle':
                node_index_list = self.get_element_node_index3(i)
                points = self.get_node_xy(node_index_list)
                dxy = self.get_node_displacement_xy(node_index_list)
                points = points + dxy * scale
                polygon = Polygon(points, closed=True)
                patches.append(polygon)
        p = PatchCollection(patches, linewidths=0.5, edgecolor='k', alpha=0.7)
        ax.add_collection(p)
        plt.title("Displacement")
        plt.xlabel("x")
        plt.ylabel("y")
        plt.axis('square')
        plt.savefig("figures/displacement_figure.pdf", format="pdf", bbox_inches="tight")
        plt.show()

    def draw_stress(self, scale=1.0):
        fig, ax = plt.subplots()
        patches = []
        stress = []
        for i in range(len(self.elements['element_key'])):
            element_name = self.elements['element_name'][i]
            if element_name == 'CSTriangle':
                stress.append(self.get_element_stress(i))
                node_index_list = self.get_element_node_index3(i)
                points = self.get_node_xy(node_index_list)
                dxy = self.get_node_displacement_xy(node_index_list)
                points = points + dxy * scale
                polygon = Polygon(points, closed=True)
                patches.append(polygon)
        p = PatchCollection(patches, linewidths=0.5, edgecolor='k', alpha=0.7, cmap=matplotlib.cm.jet)
        ax.add_collection(p)
        p.set_array(stress)
        fig.colorbar(p, ax=ax)
        plt.title("Von Mises Stress")
        plt.xlabel("x")
        plt.ylabel("y")
        plt.axis('square')
        plt.savefig("figures/stress_figure.pdf", format="pdf", bbox_inches="tight")
        plt.show()


if __name__ == '__main__':
    elements_csv_path = 'result/elements.csv'
    nodes_csv_path = 'result/nodes.csv'
    post_processing = PostProcessing(elements_csv_path, nodes_csv_path)
    post_processing.draw_fem_mesh()
    post_processing.draw_displacement(300)
    post_processing.draw_stress(300)

