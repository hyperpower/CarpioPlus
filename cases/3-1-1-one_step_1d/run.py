import os
import sys
import string
import math
import operator
import shutil
import sys

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    "plot.py",
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

def build():
    print("cmake --------------------------------- ")
    # cmake ====
    os.system("mkdir data")
    os.system("mkdir fig")
    os.system("cmake .")
    print("make  --------------------------------- ")
    os.system("make")
    print("run   --------------------------------- ")
    os.system("./build/main ")
    print("plot   -------------------------------- ")
    os.system("python3 plot.py")

def main():
    RT.clean(PATH_THIS, FILE_ORIGINAL)
    build()

if __name__ == '__main__':
    main()
