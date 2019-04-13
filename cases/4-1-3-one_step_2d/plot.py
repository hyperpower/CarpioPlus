import matplotlib
import matplotlib.pyplot as plt
import os, sys
import numpy as np
import string
import math
import operator

matplotlib.rcParams['text.usetex']        = True
matplotlib.rcParams['font.family']        = 'serif'
matplotlib.rcParams['font.size']          = 12

PATH_CASES    = os.path.abspath(os.path.join(__file__, "../.."))
PATH_THIS     = os.path.abspath(os.path.join(__file__, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS, "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS, "fig"))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))

sys.path.append(PATH_PYTOOLS)
import filetool as FT

def plot_illustration_fig():
    plt.figure(figsize=(6, 4))
    ax = plt.axes()

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'y')

    """
    Set range
    """
    x_st = -0.45
    x_ed = 1.45
    y_st = -0.35
    y_ed = 1.35

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    box_x      = [0.0, 1.0, 1.0, 0.0, 0.0]
    box_y      = [0.0, 0.0, 1.0, 1.0, 0.0]
    # dash line
    line_x     = [0.0, 1.0]
    line_y     = [0.0, 1.0]

    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(box_x  , box_y, linewidth=2.0)
    plt.plot(line_x,  line_y, "--", linewidth=2.0)
    

    plt.text(0.35, 1.05, r'$\frac{d \phi}{d y}$ = ' + "%.1f" % float(0.0), fontsize = 'large')
    plt.text(0.35,-0.15, r'$\phi$ = ' + "%.1f" % float(0.0), fontsize = 'large')
    plt.text(-0.4, .5,   r'$\phi$ = ' + "%.1f" % float(1.0), fontsize = 'large')
    plt.text(1.1,  .5,   r'$\frac{d \phi}{d x}$ = ' + "%.f" % float(1.0), fontsize = 'large')

    plt.arrow(0.5,  0.1, 0.2, 0.0, width=0.01, color = "k")
    plt.text( 0.52, 0.01, "u=1.0", fontsize = 'large')
    plt.arrow(0.5,  0.1, 0.0, 0.2, width=0.01, color = "k")
    plt.text( 0.42, 0.33, "v=1.0", fontsize = 'large')
    plt.arrow(0.5,  0.1, 0.2, 0.2, width=0.01, color = "r")

    plt.text( 0.25, 0.75, r'$\phi=1.0$', fontsize = 'large')
    plt.text( 0.6 , 0.45, r'$\phi=0.0$', fontsize = 'large')

    # plt.legend(llg, scheme, loc= 'upper right')

    # plt.grid(True)
    ax.set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()
    # plt.show()

def main():
    plot_illustration_fig()

if __name__ == '__main__':
    main()

