from Utils.Sci import von_mises_stress


class Element:
    def __init__(self):
        self.Id = 0
        self.Key = 0
        self.Name = ""
        self.NodeCount = 0
        self.Node1Id = 0
        self.Node2Id = 0
        self.Node3Id = 0
        self.Node4Id = 0
        self.Node5Id = 0
        self.Node6Id = 0
        self.Node7Id = 0
        self.Node8Id = 0
        self.StressX = 0.0
        self.StressY = 0.0
        self.StressZ = 0.0
        self.StressXY = 0.0
        self.StressYZ = 0.0
        self.StressZX = 0.0

    def get_node_id_list(self):
        node_ids = [self.Node1Id, self.Node2Id, self.Node3Id, self.Node4Id, self.Node5Id, self.Node6Id, self.Node7Id,
                    self.Node8Id]
        return node_ids[:self.NodeCount]

    def get_stress(self):
        return von_mises_stress(self.StressX, self.StressY, self.StressXY)
