import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from Utils.DataLoader import *
from Common.Node import *
from Common.Element import *


class PostDrawing:
    def __init__(self, elements_csv_path, nodes_csv_path):
        self.Elements = read_elements(elements_csv_path)
        self.Nodes = read_nodes(nodes_csv_path)
        nodes = self.Nodes.values()
        self.XMin, self.XMax = min(node.X for node in nodes), max(node.X for node in nodes)
        self.YMin, self.YMax = min(node.Y for node in nodes), max(node.Y for node in nodes)
        self.ZMin, self.ZMax = min(node.Z for node in nodes), max(node.Z for node in nodes)
        self.Fig = plt.figure()
        self.Ax = Axes3D(self.Fig, auto_add_to_figure=False)
        self.Fig.add_axes(self.Ax)

    def set_axis_lim(self):
        self.Ax.set_xlim(self.XMin, self.XMax)
        self.Ax.set_ylim(self.YMin, self.YMax)
        self.Ax.set_zlim(self.ZMin, self.ZMax)
        limits = np.array([getattr(self.Ax, f'get_{axis}lim')() for axis in 'xyz'])
        self.Ax.set_box_aspect(np.ptp(limits, axis=1))

    @staticmethod
    def show():
        plt.show()

    @staticmethod
    def save(path, fmt="pdf"):
        plt.savefig(path, format=fmt, bbox_inches="tight")


class MeshDrawing(PostDrawing):
    def __init__(self, elements_csv_path, nodes_csv_path):
        super().__init__(elements_csv_path, nodes_csv_path)

    def draw(self):
        for elem in self.Elements.values():
            node_id_list = elem.get_node_id_list()
            node_list = [self.Nodes[node_id] for node_id in node_id_list]
            verts = [tuple(node.get_position()) for node in node_list]
            self.Ax.add_collection3d(Poly3DCollection([verts], edgecolors='k', linewidths=0.5, alpha=0.5))
        plt.title("Mesh")
        plt.xlabel("x")
        plt.ylabel("y")
        self.set_axis_lim()
        return self


class DisplacementDrawing(PostDrawing):
    def __init__(self, elements_csv_path, nodes_csv_path):
        super().__init__(elements_csv_path, nodes_csv_path)

    def draw(self, scale=1.0):
        for elem in self.Elements.values():
            node_id_list = elem.get_node_id_list()
            node_list = [self.Nodes[node_id] for node_id in node_id_list]
            verts = [tuple(node.get_position() + node.get_displacement() * scale) for node in node_list]
            self.Ax.add_collection3d(Poly3DCollection([verts], edgecolors='k', linewidths=0.5, alpha=0.5, cmap=matplotlib.cm.jet))
        plt.title("Displacement")
        plt.xlabel("x")
        plt.ylabel("y")
        self.set_axis_lim()
        return self
