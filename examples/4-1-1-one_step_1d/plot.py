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
# matplotlib.rcParams['text.latex.unicode'] = True
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

def plot_one(strstep, strtime):
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
    x_st = -10
    x_ed = 100
    y_st = -0.5
    y_ed = 1.5

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    fne = PATH_DATA + "/phi_" + strstep + "_" + strtime + ".txt"

    pe    = FT.PointData(fne)
    arrx  = pe.get_coo_x()
    arre  = pe.get_arr_val()
    arrv  = []    

    plt.plot(arrx, arre) 


    plt.text(10, 1.25, "Time = "+ "%.2f" % float(strtime))
    plt.text(10, 1.00, "Step = "+ "%04d" % float(strstep))

    # plt.legend(llg, scheme, loc= 'upper right')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/comp_" + "%06d" % int(strstep) +".png")
    plt.close()
    # plt.show()

def read_error_file(scheme):
    filename = PATH_RESULT + "/error_" + scheme
    f = open(filename, 'r')
    content=[]
    for i, line in enumerate(f):
        line = line.strip()
        line = line.split()
        step = float(line[1])
        t    = float(line[3])
        e1   = float(line[5])
        e2   = float(line[7])
        ei   = float(line[9])
        content.append([step, t, e1, e2, ei])
    return content

def plot_error1():
    plt.figure(figsize=(4, 4))

    """
    Set labels
    """
    plt.ylabel(r'Error$_1$')
    plt.xlabel(r'Step')

    """
    Set range
    """
    x_st = 0
    x_ed = 200
    y_st = -0.5
    y_ed = 1.5

    #plt.xlim([x_st, x_ed])
    #plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    scheme = ["upwind1", "center", "center4"]
    mat = []
    for s in scheme:
        m = read_error_file(s)
        mat.append(m)
    llg = []
    for i, v in enumerate(scheme):
        arrx  = _col(mat[i], 0)
        arre1 = _col(mat[i], 2)
        arre2 = _col(mat[i], 3)
        arrei = _col(mat[i], 4)

        lg, = plt.plot(arrx, arre1) 
        llg.append(lg)

    #plt.text(10, 1.25, "Time = "+ "%.2f" % float(strtime))
    #plt.text(10, 1.00, "Step = "+ "%04d" % float(strstep))

    plt.legend(llg, scheme, loc= 'upper left')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig("error_1.png")
    plt.close()
    # plt.show()

def plot_error2():
    plt.figure(figsize=(4, 4))

    """
    Set labels
    """
    plt.ylabel(r'Error$_2$')
    plt.xlabel(r'Step')

    """
    Set range
    """
    x_st = 0
    x_ed = 200
    y_st = -0.5
    y_ed = 1.5

    #plt.xlim([x_st, x_ed])
    #plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    scheme = ["upwind1", "center", "center4"]
    mat = []
    for s in scheme:
        m = read_error_file(s)
        mat.append(m)
    llg = []
    for i, v in enumerate(scheme):
        arrx  = _col(mat[i], 0)
        arre1 = _col(mat[i], 2)
        arre2 = _col(mat[i], 3)
        arrei = _col(mat[i], 4)

        lg, = plt.plot(arrx, arre2) 
        llg.append(lg)

    #plt.text(10, 1.25, "Time = "+ "%.2f" % float(strtime))
    #plt.text(10, 1.00, "Step = "+ "%04d" % float(strstep))

    plt.legend(llg, scheme, loc= 'upper left')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig("error_2.png")
    plt.close()

def plot_errori():
    plt.figure(figsize=(4, 4))

    """
    Set labels
    """
    plt.ylabel(r'Error$_{\inf}$')
    plt.xlabel(r'Step')

    """
    Set range
    """
    x_st = 0
    x_ed = 200
    y_st = -0.5
    y_ed = 1.5

    #plt.xlim([x_st, x_ed])
    #plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    scheme = ["upwind1", "center", "center4"]
    mat = []
    for s in scheme:
        m = read_error_file(s)
        mat.append(m)
    llg = []
    for i, v in enumerate(scheme):
        arrx  = _col(mat[i], 0)
        arre1 = _col(mat[i], 2)
        arre2 = _col(mat[i], 3)
        arrei = _col(mat[i], 4)

        lg, = plt.plot(arrx, arrei) 
        llg.append(lg)

    #plt.text(10, 1.25, "Time = "+ "%.2f" % float(strtime))
    #plt.text(10, 1.00, "Step = "+ "%04d" % float(strstep))

    plt.legend(llg, scheme, loc= 'upper left')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig("error_i.png")
    plt.close()

def plot_all():
    matfu = file_name(PATH_RESULT, "exact")
    print(len(matfu))
    matfc = []
    for one in matfu:
        matfc.append(one)

    #multiprocessing.freeze_support()
    pool = multiprocessing.Pool()
    cpus = multiprocessing.cpu_count() / 2
    results = []
    cmatfs = split(matfc, cpus)

    print("Thread num : ", len(cmatfs))
    for i in xrange(0, cpus):
        mat = cmatfs[i]
        for one in mat:
            result = pool.apply_async(plot_one, args=(one[1], one[2],))
            results.append(result)
    
    pool.close()
    pool.join()
    
    os.system("convert -delay 5 -loop 0 ./fig/comp_*.png comp.gif")

def mp():
    lists=[1,2,3]
    pool=Pool(processes=10) # the max processes number 
    for i in range(0, 50):
        pool.apply_async(test,args=(i,))
    pool.close()
    pool.join()

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
    arrx_start = [x_st, 0.5, 0.5, x_ed] 
    arr_v      = [1.0,  1.0, 0.0, 0.0]
    arrx_end   = [x_st, 1.5, 1.5, x_ed]    

    plt.plot(arrx_end  , arr_v, linewidth=2.0)
    plt.plot(arrx_start, arr_v, linewidth=3.0)

    plt.text(0.25, 1.05, "Time = "+ "%.2f" % float(0.0), fontsize = 'large')
    plt.text(1.25, 1.05, "Time = "+ "%.2f" % float(1.0), fontsize = 'large')

    plt.annotate('Initial profile', 
            xy=(0.5, 0.75), xytext=(0.75, 0.85), fontsize = 'large',
            arrowprops = dict(facecolor='black', shrink = 0.05),
            )
    plt.annotate('Advected profile', 
            xy=(1.5, 0.35), xytext=(0.6, 0.2), fontsize = 'large',
            arrowprops = dict(facecolor='black', shrink = 0.05),
            )
    plt.arrow(0.4, 0.5, 0.2, 0.0, width=0.01, color = "k")

    plt.text(0.52, 0.55, "u = 1.0", fontsize = 'large')

    # plt.legend(llg, scheme, loc= 'upper right')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()
    # plt.show()

def main():
    plot_illustration_fig()
    # stri = "4"
    # strt = "4.0000e-02"
    # plot_one(stri, strt) 
    
    #plot_all()
    #plot_error1()
    #plot_error2()
    #plot_errori()

if __name__ == '__main__':
    main()


