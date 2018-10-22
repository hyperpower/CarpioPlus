import os
import sys
import string
import math
import operator

_DIR_THIS_  = os.path.dirname(__file__)
_DIR_CASES_ = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
_PY_TOOLS_  = os.path.join(_DIR_CASES_, "pytools");
sys.path.insert(0, _PY_TOOLS_)
import runtool

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    "plot.py",
    "run.py"
]

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
    os.system("python plot.py")

def main():
    runtool.clean(_DIR_THIS_, FILE_ORIGINAL)
    build()

if __name__ == '__main__':
    main()