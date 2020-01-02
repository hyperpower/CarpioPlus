import matplotlib
import matplotlib.pyplot as plt
import os, sys
import numpy as np
import string
import math
import operator

import multiprocessing
from multiprocessing import Pool

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

def file_name(namedir, namevar):
    res = []
    files = [f for f in os.listdir(namedir) if os.path.isfile(os.path.join(namedir, f))]
    for f in files:
        spf = f.split("_")
        if spf[0] == namevar:
            res.append(spf)
    return res 

def split(seq, num):
    avg = len(seq) / float(num)
    out = []
    last = 0.0

    while last < len(seq):
        out.append(seq[int(last):int(last + avg)])
        last += avg

    return out

def exact_value(x, t):
    xc = 0.5 + t;
    if x > (xc - 0.25) and x < (xc + 0.25):
        return 1.0;
    else:
        return 0.0;

def add_exact(plt, strstep, strtime):
    step = float(strstep)
    time = float(strtime)

    xc   = 0.5
    xnew = xc + time * 1.0 # v = 1.0

    x    = [0.0, 
            xnew - 0.25,
            xnew - 0.25, 
            xnew + 0.25,
            xnew + 0.25,
            2.0 ]
    y    = [0.0, 0.0, 1.0, 1.0, 0.0, 0.0]

    l, = plt.plot(x, y)

    return l 

def plot_one(scheme, strstep, strtime):
    print("Draw : ", strstep, " ", strtime)

    plt.figure(figsize=(6, 4))

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'$\phi$')

    """
    Set range
    """
    x_st = 0.0
    x_ed = 2.0
    y_st = -0.2
    y_ed = 1.5

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    fne = PATH_DATA + "/" + scheme + "_phi_" + strstep + "_" + strtime + ".txt"

    pe    = FT.PointData(fne)
    arrx  = pe.get_coo_x()
    arre  = pe.get_arr_val()
    arrv  = []    

    lexact = add_exact(plt, strstep, strtime)

    l,     = plt.plot(arrx, arre) 

    plt.text(0.25, 1.35, "Time = "+ "%.2f" % float(strtime))
    plt.text(0.25, 1.10, "Step = "+ "%04d" % float(strstep))

    plt.legend([l,lexact], [scheme, "Exact"], loc= 'upper right')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/" + scheme + "_%06d" % int(strstep) +".png")
    plt.close()
    # plt.show()

def plot_illustration_fig():
    plt.figure(figsize=(6, 4))

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'$\phi$')

    """
    Set range
    """
    x_st = 0
    x_ed = 2
    y_st = -0.25
    y_ed = 1.25

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(arrx_end  , arr_v, linewidth=2.0)
    plt.plot(arrx_start, arr_v, linewidth=3.0)

    plt.text(0.25, 1.05, "Time = "+ "%.2f" % float(0.0))
    plt.text(1.25, 1.05, "Time = "+ "%.2f" % float(1.0))

    plt.annotate('Initial profile', 
            xy=(0.26, 0.75), xytext=(0.55, 0.85),
            arrowprops = dict(facecolor='black', shrink = 0.05),
            )
    plt.annotate('Advected profile', 
            xy=(1.75, 0.35), xytext=(0.9, 0.2),
            arrowprops = dict(facecolor='black', shrink = 0.05),
            )
    plt.arrow(0.4, 0.5, 0.2, 0.0, width=0.01, color = "k")

    plt.text(0.42, 0.55, "u = 1.0")

    # plt.legend(llg, scheme, loc= 'upper right')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()
    # plt.show()

def plot_all(scheme):
    fmat = file_name("./data/", scheme)
    # row of fmat
    # [scheme, phi, step, time.txt]
    for f in fmat:
        strstep = f[2]
        strtime = f[3][:-4]
        plot_one(scheme, strstep, strtime)

    # make gif
    os.system("convert -delay 5 -loop 0 ./fig/%s_*.png ./fig/%s.gif" % (scheme, scheme))

def cal_norm(scheme, strstep, strtime):
    fne = PATH_DATA + "/" + scheme + "_phi_" + strstep + "_" + strtime + ".txt"

    pe    = FT.PointData(fne)
    arrx  = pe.get_coo_x()
    arrv  = pe.get_arr_val()
    arre  = []    
    for row in zip(arrx, arrv):
        x = row[0]
        v = row[1]
        exact = exact_value(x, float(strtime))
        error = v - exact
        arre.append(error)

    n1 = np.linalg.norm(arre, 1)
    n2 = np.linalg.norm(arre)
    ni = np.linalg.norm(arre, np.inf)

    return (n1, n2, ni)

def plot_norm(scheme):
    plt.figure(figsize=(6, 4))

    """
    Set labels
    """
    plt.xlabel(r'step (n)')
    plt.ylabel(r'Norm')

    """
    Set range
    """
    x_st = 0.0
    x_ed = 100
    y_st = -1.5
    y_ed = 1.5

    plt.xlim([x_st, x_ed])
    # plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')
    plt.title(scheme)
    """
    Data part
    """
    step = []
    ln1  = []
    ln2  = []
    lni  = []

    fmat = file_name("./data/", scheme)
    # row of fmat
    # [scheme, phi, step, time.txt]
    for f in fmat:
        strstep = f[2]
        strtime = f[3][:-4]
        # plot_one(scheme, strstep, strtime)
        n1, n2, ni = cal_norm(scheme, strstep, strtime)
        ln1.append(n1)
        ln2.append(n2)
        lni.append(ni)
        step.append(int(strstep))

        if int(strstep) == 100:
            textboxprop = dict(facecolor = "white", alpha = 0.8)
            plt.text(60, n1, "Norm1 = "+ "%.4e" % float(n1), va="center", bbox=textboxprop)
            plt.text(60, n2, "Norm2 = "+ "%.4e" % float(n2), va="center", bbox=textboxprop)
            plt.text(60, ni, "Normi = "+ "%.4e" % float(ni), va="center", bbox=textboxprop)

    l1, = plt.plot(step, ln1, ".")
    l2, = plt.plot(step, ln2, ".")
    li, = plt.plot(step, lni, ".")

    # plt.text(0.25, 1.35, "Time = "+ "%.2f" % float(strtime))
    # plt.text(0.25, 1.10, "Step = "+ "%04d" % float(strstep))

    plt.legend([l1, l2, li], ["Norm1","Norm2","Norminf"], loc= 'best')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/" + scheme + "_norm.png")
    plt.close()
    # plt.show()


def main():
    plot_illustration_fig()

    # plot_all("QUICK")
    plot_norm("FOU")
    plot_norm("QUICK")

if __name__ == '__main__':
    main()


