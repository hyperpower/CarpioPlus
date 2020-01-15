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

def velocity(x, y):
    return (-(y-40)*2*math.pi/360.0, (x-40)*2*math.pi/360.0)

def add_stream_line(ax, xst, yst, step=10):
    dt   = 1
    arrx = [xst]
    arry = [yst]

    for i in range(0,step):
        x   = arrx[i]
        y   = arry[i]
        u,v = velocity(x, y)
        nx  = x + u * dt 
        ny  = y + v * dt 
        arrx.append(nx)
        arry.append(ny)

    ax.plot(arrx, arry, linewidth=2.0, color = "C1")


def plot_illustration_fig():
    plt.figure(figsize=(8, 5))
    ax = plt.axes()

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'y')

    """
    Set range
    """
    x_st = -15
    x_ed = 90
    y_st = -10
    y_ed = 90

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    box_x      = [ 0.0, 80.0, 80.0, 0.0 , 0.0]
    box_y      = [ 0.0, 0.0,  80.0, 80.0, 0.0]

    cx = 20
    cy = 40
    hl = 4  

    ib_x      = [ cx - hl, cx + hl, cx + hl, cx - hl , cx - hl]
    ib_y      = [ cy - hl, cy - hl, cy + hl, cy + hl , cy - hl]

    plt.plot(ib_x, ib_y, linewidth=2.0, color = "k")

    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(box_x  , box_y, linewidth=2.0)

    add_stream_line(ax, 20.0, 40.0, 100)
    # add_stream_line(ax, -0.6, 0.0, 100)
    # add_stream_line(ax, -0.4, 0.0, 100)
    # add_stream_line(ax, -0.2, 0.0, 100)

    # the 1/6 ticky
    ticky = 20
    plt.plot([ticky, ticky],[-2, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, -5.0, "%.1f" % ticky, color = "C0", ha="center",va = "center")
    ticky = 16 
    plt.plot([ticky, ticky],[2, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, 5.0, "%.f" % ticky, color = "C0", ha="center",va = "center")
    ticky = 24 
    plt.plot([ticky, ticky],[2, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, 5.0, "%.f" % ticky, color = "C0", ha="center",va = "center")

    ticky = 36
    plt.plot([2, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( 8.0, ticky, "%.1f" % ticky, color = "C0", ha="center",va = "center")
    ticky = 44 
    plt.plot([2, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( 8.0, ticky, "%.1f" % ticky, color = "C0", ha="center",va = "center")

    ticky = 40
    plt.plot([-2, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( -8.0, ticky, "%.1f" % ticky, color = "C0", ha="center",va = "center")

    vv = {"x" : 40, "y" : 70}
    plt.text( vv["x"], vv["y"], r'$u=-(y-40)\frac{2 \pi}{360}$', ha="center")
    vv = {"x" : 40, "y" : 60}
    plt.text( vv["x"], vv["y"], r'$v=(x-40)\frac{2 \pi}{360}$', ha="center")
    

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

def find_section_file(namedir, namescheme, named, namevar):
    res = []
    files = [f for f in os.listdir(namedir) if os.path.isfile(os.path.join(namedir, f))]
    for f in files:
        spf = f.split("_")
        if spf[0]     == namescheme \
           and spf[1] == named \
           and spf[2] == namevar:
            res.append(f)
    return res 

def add_section_exact(plt, y):
    arrx = np.linspace(0.0, 1.0, num=100) 
    arry = []
    x1 = math.sqrt(0.6 * 0.6 - y * y)
    x2 = math.sqrt(0.8 * 0.8 - y * y)
    x3 = math.sqrt(0.2 * 0.2 - y * y)
    x4 = math.sqrt(0.4 * 0.4 - y * y)
    for value in arrx:
        if x1 < value < x2 or x3 < value < x4:
            arry.append(1.0)
        else:
            arry.append(0.0) 

    l, = plt.plot(arrx, arry, color = "k")
    return l

def add_a_scheme(ax, named, scheme):
    file = find_section_file("./data/", scheme, named, "phi")
    if len(file) < 1:
        return

    fd   = FT.TextFile(PATH_DATA + "/" + file[0])
    d    = fd.get_data()
    ncol = 1
    if named == "x":
        ncol = 2
    l,   = ax.plot(FT.col(d, ncol), FT.col(d, 4), ".")

    ncol = 2
    if named == "x":
        ncol = 1
    avg = np.average(FT.col(d, ncol)) 

    return l, avg

def plot_setion_compare(d, schemes):
    fig = plt.figure(figsize=(6, 5))
    ax  = fig.add_subplot(111)

    """
    Set labels
    """
    nd = "X"
    if d == "x":
        nd = "Y"
    plt.xlabel(nd)
    plt.ylabel(r'$\phi$')
    # plt.zlabel(r'$\phi$')

    """
    Set range
    """
    x_st = 0.0
    x_ed = 80.0
    y_st = 0.0
    y_ed = 1.2

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    # plt.yscale('log')

    """
    Data part
    """
    ls = []
    lname = []
    for scheme in schemes:
        l, avgx = add_a_scheme(ax, d, scheme)
        ls.append(l)
        lname.append(scheme)
    
    # lexact = add_section_exact(plt, avgx)
    # ls.append(lexact)
    # lname.append("Exact")
    plt.legend(ls, lname, loc= 'best')

    plt.text(70,  0.23, d + r' = %.2f' % avgx, ha = "center")

    plt.grid(True)
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/compare_section_%s.png" % d)
    # plt.show()
    plt.close()

def main():
    plot_illustration_fig()
    arrscheme = [
        "FOU",
        "Superbee",
        "Minmod"
    ]
    plot_setion_compare("x", arrscheme)
    plot_setion_compare("y", arrscheme)
    make_gif("FOU")

if __name__ == '__main__':
    main()


