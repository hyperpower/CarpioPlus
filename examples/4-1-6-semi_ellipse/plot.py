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
    line2_x    = [0.0,       1.0 - (1.0/6.0)]
    line2_y    = [(1.0/6.0), 1.0]
    # dash line 2
    line3_x    = [(1.0/6.0), 1.0]
    line3_y    = [0.0, 1.0 - (1.0/6.0)]

    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(box_x  , box_y, linewidth=2.0)

    plt.plot(line_x,   line_y, "--", linewidth=2.0)
    plt.plot(line2_x, line2_y, "--", linewidth=2.0, color = "C1")
    plt.plot(line3_x, line3_y, "--", linewidth=2.0, color = "C1")

    # the 1/6 ticky
    ticky = 1.0/6.0
    plt.plot([-0.05, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( -0.2, ticky, "1/6", color = "C0", va = "center")
    
    plt.text(-0.4, .08, r'$\sqrt{1 - (6 * y)^2}$', va="center")

    plt.plot([ticky, ticky], [-0.05, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text( ticky, -0.14, "1/6", color = "C0",   ha="center")

    plt.text(-0.3, -.1, r'$\sqrt{1 - (6 * x)^2}$', va="center")

    plt.text(0.25, 1.05, r'$\frac{d \phi}{d y}$ = ' + "%.f" % float(0.0))
    plt.text(0.25,-0.15, r'$\phi$ = ' + "%.1f" % float(0.0))
    plt.text(-0.35, .6,  r'$\phi$ = ' + "%.1f" % float(0.0), va="center")
    plt.text(1.1,  .5,   r'$\frac{d \phi}{d x}$ = ' + "%.f" % float(0.0))

    vv = {"x" : 0.6, "y" : 0.1}
    plt.arrow(vv["x"],        vv["y"], 0.2, 0.0, width=0.01, color = "k")
    plt.text( vv["x"] + 0.06, vv["y"] - 0.07, "u = 1.0")
    plt.arrow(vv["x"],        vv["y"], 0.0, 0.2, width=0.01, color = "k")
    plt.text( vv["x"] - 0.1,  vv["y"] + 0.25, "v = 1.0")
    plt.arrow(vv["x"],        vv["y"], 0.2, 0.2, width=0.01, color = "r")

    plt.text( 0.75, 0.45, r'$\phi=0.0$')
    plt.text( 0.1, 0.75, r'$\phi=0.0$')

    # plot cut
    xcut = 0.7
    plt.arrow(xcut,  1.1,  0.1, 0.0, head_width=0.05, color = "k")
    plt.arrow(xcut,  -0.1, 0.1, 0.0, head_width=0.05, color = "k")
    plt.plot([xcut, xcut], [1.1, 1.05], color = "k")
    plt.plot([xcut, xcut], [-0.1, -.05], color = "k")
    plt.text(xcut + 0.2,  1.1, r'A-A (x = 0.7)', va = "center")
    plt.text(xcut + 0.2, -0.1, r'A-A (x = 0.7)', va = "center")
    # plt.legend(llg, scheme, loc= 'upper right')

    # plt.grid(True)
    ax.set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()
    # plt.show()

def file_name(namedir, namescheme, namevar):
    res = []
    files = [f for f in os.listdir(namedir) if os.path.isfile(os.path.join(namedir, f))]
    for f in files:
        spf = f.split("_")
        if spf[0] == namescheme and spf[1] == namevar:
            res.append(spf)
    return res 

def make_gif(scheme):
    # the png is plot by gnuplot
    filemat = file_name("./fig/", scheme, "phi")
    if len(filemat[0]) < 4 :
        return
    sfm     = sorted(filemat, key = lambda x: int(x[2]))
    # change file name
    count = 0 
    for row in sfm:
        old_fn  = "%s_%s_%s_%s" % (row[0], row[1], row[2], row[3])
        new_fn  = "%s_%s_%06d.png" % (row[0], row[1], count)
        os.system("mv ./fig/%s ./fig/%s" % (old_fn, new_fn))
        count += 1

    os.system("convert -delay 5 -loop 0 ./fig/%s_%s_*.png ./fig/%s_%s.gif" % (scheme, "phi", scheme, "phi"));

def find_section_file(namedir, namescheme, namevar):
    res = []
    files = [f for f in os.listdir(namedir) if os.path.isfile(os.path.join(namedir, f))]
    for f in files:
        spf = f.split("_")
        if spf[0]     == namescheme \
           and spf[1] == "section" \
           and spf[2] == namevar:
            res.append(f)
    return res 

def add_section_exact(plt, x):
    arrx = np.linspace(0.0, 1.0, num=100) 
    arry = np.array([])
    v16  = 1.0 / 6.0
    for value in arrx:
        if x - v16 < value < x + v16:
            s = math.sqrt(1.0 - 36 * (value - x) * (value - x))
            arry = np.append(arry, s)
        else:
            arry = np.append(arry, 0)
    l, = plt.plot(arrx, arry, color = "k")
    return l

def add_a_scheme(ax, scheme):
    file = find_section_file("./data/", scheme, "phi")
    if len(file) < 1:
        return

    fd   = FT.TextFile(PATH_DATA + "/" + file[0])
    d    = fd.get_data()
    l1,  = ax.plot(FT.col(d, 2), FT.col(d, 4), ".")

    avgx = np.average(FT.col(d, 1)) 

    return l1, avgx

def plot_setion_compare(schemes):
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
    ls = []
    lname = []
    for scheme in schemes:
        l, avgx = add_a_scheme(ax, scheme)
        ls.append(l)
        lname.append(scheme)
    
    lexact = add_section_exact(plt, avgx)
    ls.append(lexact)
    lname.append("Exact")
    plt.legend(ls, lname, loc= 'best')

    plt.text(0.3,  0.23, r'x = %.2f' % avgx, va = "center")

    plt.grid(True)
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/compare_section.png")
    plt.close()

def main():
    plot_illustration_fig()
    arrscheme = [
        "FOU",
        "Superbee",
        "Minmod"
    ]
    plot_setion_compare(arrscheme)
    # make_gif("fou")

if __name__ == '__main__':
    main()


