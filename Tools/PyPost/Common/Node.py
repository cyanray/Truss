import numpy as np


class Node:
    def __init__(self):
        self.Id = 0
        self.Key = 0
        self.X = 0.0
        self.Y = 0.0
        self.Z = 0.0
        self.DX = 0.0
        self.DY = 0.0
        self.DZ = 0.0
        self.RX = 0.0
        self.RY = 0.0
        self.RZ = 0.0

    def get_position(self):
        return np.array([self.X, self.Y, self.Z]).T

    def get_displacement(self):
        return np.array([self.DX, self.DY, self.DZ]).T
