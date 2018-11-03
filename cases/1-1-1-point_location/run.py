import os
import sys
import time

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    # "plot.py",
    "run.py",
    "report.rstt",
    "report.py"
]



PATH_THIS     = os.path.abspath(os.path.join(__file__,   "../"))
PATH_CASES    = os.path.abspath(os.path.join(__file__,   "../.."))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS,  "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS,  "fig"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))

sys.path.append(PATH_PYTOOLS)
import runtool as RT

CASE_INFO = {}

def build():
    runtime = []
    tprev = time.clock()
    print("cmake --------------------------------- ")
    # cmake ====
    os.system("mkdir data")
    os.system("mkdir fig")
    os.system("cmake .")
    runtime.append(("cmake", time.clock() - tprev))
    tprev = time.clock()
    print("make  --------------------------------- ")
    os.system("make")
    runtime.append(("make", time.clock() - tprev))
    tprev = time.clock()
    print("run   --------------------------------- ")
    os.system("./build/main ")
    runtime.append(("run", time.clock() - tprev))
    tprev = time.clock()
    print("plot   -------------------------------- ")
    runtime.append(("plot", time.clock() - tprev))
    tprev = time.clock()
    print("report   ------------------------------ ")
    os.system("python3 report.py")
    runtime.append(("report", time.clock() - tprev))
    tprev = time.clock()
    # os.system("python3 plot.py")
    return runtime

def main():
    RT.clean(PATH_THIS, FILE_ORIGINAL)
    CASE_INFO.update(RT.case_info(PATH_THIS, FILE_ORIGINAL))
    # print(CASE_INFO)
    rt = build()
    # print(rt)

if __name__ == '__main__':
    main()