import matplotlib
import matplotlib.pyplot as plt
import os, sys
import numpy as np
import string
import math
import operator

import multiprocessing
from multiprocessing import Pool

matplotlib.rcParams['text.usetex']        = True
matplotlib.rcParams['font.family']        = 'serif'
matplotlib.rcParams['font.size']          = 14

PATH_CASES    = os.path.abspath(os.path.join(__file__, "../.."))
PATH_THIS     = os.path.abspath(os.path.join(__file__, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS, "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS, "fig"))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))

sys.path.append(PATH_PYTOOLS)
import filetool as FT

# plot figure for illustration
def figure_1():
    plt.figure(figsize=(8,6))
    frame = plt.gca()
    ax = plt.axes()
    plt.axis('off')
    ax.set_aspect("equal")
    plot_box_line(ax, box, line)

    plot_annotation(plt)

    plt.show()


box = np.array([[0.0, 0.0],
                [1.0, 0.0],
                [1.0, 1.0],
                [0.0, 1.0],
                [0.0, 0.0]])

line = np.array([[0.2,1.2],[0.8,-0.3]])


def plot_box_line(ax, box, line):
    print(box[:, 0])
    ax.plot(box[:,0], box[:,1],"-")
    ax.plot(line[:,0], line[:,1])
    
def plot_annotation(plt):
    arrowprops = dict(arrowstyle = "->",
                      connectionstyle = "arc3")
    plt.annotate("Box", xy        =(1.0, 0.9), 
                        xytext    =(1.2, 1.0),
                        va="center", 
                        ha="center",
                        arrowprops=arrowprops)


def main():
    figure_1()

if __name__ == '__main__':
    main()