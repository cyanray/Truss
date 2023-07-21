import numpy as np


def von_mises_stress(x, y, xy):
    return np.sqrt(x ** 2 - x * y + y ** 2 + 3 * xy ** 2)
