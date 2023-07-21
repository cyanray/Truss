from PyPost.PostDrawing import *
import matplotlib

matplotlib.use('Qt5Agg')

elements_path = "E:/test/result2/elements.csv"
nodes_path = "E:/test/result2/nodes.csv"

mesh = MeshDrawing(elements_path, nodes_path)
mesh.draw().show()

disp = DisplacementDrawing(elements_path, nodes_path)
disp.draw(100.0).show()
