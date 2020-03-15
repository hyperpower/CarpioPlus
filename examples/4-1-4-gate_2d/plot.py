import matplotlib
import matplotlib.pyplot as plt
import os, sys
import numpy as np
import string
import math
import operator

# matplotlib.rcParams['text.usetex']        = True
matplotlib.rcParams['font.family']        = 'monospace'
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
    plt.figure(figsize=(6, 5))
    ax = plt.axes()

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'y')

    """
    Set range
    """
    x_st = -0.4
    x_ed = 1.4
    y_st = -0.3
    y_ed = 1.3

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
    # dash line 2
    line2_x    = [0.0, 0.7]
    line2_y    = [0.3, 1.0]

    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(box_x  , box_y, linewidth=2.0)

    plt.plot(line_x,   line_y, "--", linewidth=2.0)
    plt.plot(line2_x, line2_y, "--", linewidth=2.0, color = "C1")

    # the 0.3 tick
    plt.plot([-0.05, 0.0], [0.3, 0.3], "-", linewidth=2.0, color = "C0")
    plt.text( -0.2, 0.3, "0.3", color = "C0", va = "center")

    plt.text(0.25, 1.05, r'$\frac{d \phi}{d y}$ = ' + "%.f" % float(0.0))
    plt.text(0.25,-0.15, r'$\phi$ = ' + "%.1f" % float(0.0))
    plt.text(-0.35, .15,  r'$\phi$ = ' + "%.1f" % float(1.0), va="center")
    plt.text(-0.35, .6,   r'$\phi$ = ' + "%.1f" % float(0.0), va="center")
    plt.text(1.1,  .5,   r'$\frac{d \phi}{d x}$ = ' + "%.f" % float(0.0))

    plt.arrow(0.5,  0.1, 0.2, 0.0, width=0.01, color = "k")
    plt.text( 0.52, 0.01, "u = 1.0")
    plt.arrow(0.5,  0.1, 0.0, 0.2, width=0.01, color = "k")
    plt.text( 0.42, 0.34, "v = 1.0")
    plt.arrow(0.5,  0.1, 0.2, 0.2, width=0.01, color = "r")

    plt.text( 0.5, 0.75, r'$\phi=1.0$')
    plt.text( 0.6, 0.45, r'$\phi=0.0$')
    plt.text( 0.1, 0.75, r'$\phi=0.0$')

    # plot cut
    xcut = 0.6
    plt.arrow(xcut,  1.1,  0.1, 0.0, head_width=0.05, color = "k")
    plt.arrow(xcut,  -0.1, 0.1, 0.0, head_width=0.05, color = "k")
    plt.plot([xcut, xcut], [1.1, 1.05], color = "k")
    plt.plot([xcut, xcut], [-0.1, -.05], color = "k")
    plt.text(xcut + 0.2,  1.1, r'A-A (x = 0.6)', va = "center")
    plt.text(xcut + 0.2, -0.1, r'A-A (x = 0.6)', va = "center")
    # plt.legend(llg, scheme, loc= 'upper right')

    # plt.grid(True)
    ax.set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()
    # plt.show()

def plot_norm_fig(scheme):
    plt.figure(figsize=(6, 5))
    ax = plt.axes()

    """
    Set labels
    """
    plt.xlabel(r'Step')
    plt.ylabel(r'Norm')

    """
    Set range
    """
    x_st = -0.45
    x_ed = 1.45
    y_st = -0.35
    y_ed = 1.35

    # plt.xlim([x_st, x_ed])
    # plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    plt.yscale('log')

    """
    Data part
    """
    # file check
    if not os.path.exists(PATH_DATA + "/%s_norm1.txt" % scheme):
        return
    # read file
    file = FT.TextFile(PATH_DATA + "/%s_norm1.txt" % scheme)
    d    = file.get_data()
    l1,  = ax.plot(FT.col(d, 1), FT.col(d, 2))
    file = FT.TextFile(PATH_DATA + "/%s_norm2.txt" % scheme)
    d    = file.get_data()
    l2,  = ax.plot(FT.col(d, 1), FT.col(d, 2))
    file = FT.TextFile(PATH_DATA + "/%s_norminf.txt" % scheme)
    d    = file.get_data()
    li,  = ax.plot(FT.col(d, 1), FT.col(d, 2))
    plt.legend([l1,l2,li], ["Norm1", "Norm2", "Norminf"], loc= 'best')

    plt.grid(True)
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/%s_norm.png" % scheme)
    plt.close()
    # plt.show()

def make_gif(scheme):
    # the png is plot by gnuplot
    filemat = FT.select_files_split("./fig/", scheme, "phi")
    if len(filemat[0]) < 4 :
        return
    sfm     = sorted(filemat, key = lambda x: int(x[2]))
    # change file name
    count = 0 
    nfiles = []
    for row in sfm:
        old_fn  = "%s_%s_%s_%s" % (row[0], row[1], row[2], row[3])
        new_fn  = "%s_%s_%06d.png" % (row[0], row[1], count)
        nfiles.append(new_fn)
        os.system("mv ./fig/%s ./fig/%s" % (old_fn, new_fn))
        count += 1

    os.system("convert -delay 5 -loop 0 ./fig/%s_%s_*.png ./fig/%s_%s.gif" % (scheme, "phi", scheme, "phi"));

    for i in range(0, len(nfiles)):
        if i != len(nfiles) - 1:
            os.system("rm ./fig/" + nfiles[i])
        else:
            os.system("mv ./fig/" + nfiles[i] + " ./fig/"+scheme +"_phi_last.png")

def find_section_file(namedir, namescheme, namevar):
    res = []
    files = [f for f in os.listdir(namedir) if os.path.isfile(os.path.join(namedir, f))]
    for f in files:
        spf = f.split("_")
        if spf[0]     == namescheme \
           and spf[1] == "Section" \
           and spf[2] == namevar:
            res.append(f)
    return res 

def add_section_exact(plt, x):
    y1 = x
    y2 = x + 0.3
    l, = plt.plot([0.0, y1, y1, y2, y2, 1.0], [0.0, 0.0, 1.0, 1.0, 0.0, 0.0], color = "k")
    return l

def plot_setion(scheme):
    plt.figure(figsize=(6, 5))
    ax = plt.axes()

    """
    Set labels
    """
    plt.xlabel(r'Y')
    plt.ylabel(r'$\phi$')

    """
    Set range
    """
    x_st = 0.2
    x_ed = 1.0
    y_st = -0.35
    y_ed = 1.35

    plt.xlim([x_st, x_ed])
    # plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    # plt.yscale('log')

    """
    Data part
    """
    file = find_section_file("./data/", scheme, "phi")
    if len(file) < 1:
        return

    fd   = FT.TextFile(PATH_DATA + "/" + file[0])
    d    = fd.get_data()
    l1,  = ax.plot(FT.col(d, 2), FT.col(d, 4), ".")

    lexact = add_section_exact(plt, 0.6)
    plt.legend([l1, lexact], [scheme, "Exact"], loc= 'best')

    plt.grid(True)
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/" + scheme + "_Section.png")
    plt.close()

def plot_a_scheme(scheme):
    plot_setion(scheme)
    plot_norm_fig(scheme)
    # change_file_name(scheme)

def main():
    plot_illustration_fig()
    plot_a_scheme("FOU")
    make_gif("FOU")
    plot_a_scheme("QUICK")
    plot_a_scheme("CDS")


if __name__ == '__main__':
    main()


