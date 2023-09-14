from PyPost.PostDrawing import *
import matplotlib

matplotlib.use('Qt5Agg')

elements_path = "E:/test/Plane/result/elements.csv"
nodes_path = "E:/test/Plane/result/nodes.csv"

mesh = MeshDrawing(elements_path, nodes_path)
mesh.draw().show()

disp = DisplacementDrawing(elements_path, nodes_path)
disp.draw(100.0).show()
