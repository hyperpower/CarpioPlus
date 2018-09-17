#! /usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import unicode_literals, print_function
import os 
import re
import sys
import math
import argparse
import numpy as np
import string
import math
import operator
import numbers


def col(matrix, i):
    return [row[i] for row in matrix]

class TextFile:
    def __init__(self, filename):
        self._fn = filename
        self._read_file(filename)
        self._get_dict()
        self._get_data()

    def _read_file(self, filename):
        f = open(filename, 'r')
        self._content=[]
        for i, line in enumerate(f):
            self._content.append(line)

    def is_number(self, value):
        try:
            float(value)
            return True
        except ValueError:
            return False

    def _get_dict(self):
        self._dict = {}
        for line in self._content:
            flag, string = self._parse_line_dict(line)
            if flag:
                # trim ##
                ns = string[3:-1]
                ns.strip()
                ans = ns.split(":")
                assert(len(ans) == 2)
                key = ans[0].strip()
                val = ans[1].strip()
                self._dict[key] = val

    def _get_data(self):
        self._mat = []
        for line in self._content:
            arr = self._parse_line_data(line)
            if len(arr) > 0:
                self._mat.append(arr)

    def get_config(self):
        return self._dict

    def get_data(self):
        return self._mat

    def _parse_line_data(self, line):
        # sperated by ,
        # trim space
        arr = []
        p = re.compile(r'#.+')
        if re.match(p, line):
            return arr
        
        sl = line.split(",")
        for string in sl:
            ns = string.strip()
            if self.is_number(ns):
                arr.append(float(ns))
        return arr


    def _parse_line_dict(self, line):
        # begin with ## 
        # seperated by :
        # trim space
        p = re.compile(r'##\s.+:.+')

        m = re.match(p, line)
        if m:
            return True, m.string
        else:
            return False, ""

    def get_coo_x(self):
        nx = float(self._dict["NX"])
        arr = []
        for i in range(0, int(nx)):
            arr.append(self._mat[i][0])
        return arr

    def get_coo_y(self):
        nx  = int(self._dict["NX"])
        ny  = int(self._dict["NY"])
        dim = int(self._dict["Dim"])
        arr = []
        if dim >=2:
            for i in range(0, int(ny)):
                arr.append(self._mat[i* nx][1])
            return arr
        else:
            return [0]

    def get_mat_val(self):
        assert(int(self._dict["Dim"]) ==2)
        nx  = int(self._dict["NX"])
        ny  = int(self._dict["NY"])
        dim = int(self._dict["Dim"])
        mat = []
        count = 0
        for j in range(0, ny):
            row = []
            for i in range(0, nx):
                row.append(self._mat[count][3])
                count += 1
            mat.append(row)
        return mat

    def get_arr_val(self):
        assert(int(self._dict["Dim"]) == 1)
        nx  = int(self._dict["NX"])
        dim = int(self._dict["Dim"])
        arr = []
        for i in range(0, nx):
            arr.append(self._mat[i][3])
        return arr


class PointData(TextFile):
    def __init__(self, filename):
        TextFile.__init__(self, filename)
        self.check_file()
        self._cn = self._column_name_to_dict()

    def check_file(self):
        assert("SIZE" in self._dict)
        assert("COLUMN_NAME" in self._dict)

    def column_name(self):
        return self._cn

    def column(self, name):
        if name in self._cn:
            arr = col(self._mat, self._cn[name])
            return arr
        else:
            print("No column named :", name)
            return []

    def _column_name_to_dict(self):
        line = self._dict["COLUMN_NAME"]
        self._parse_line_data(line)
        sl = line.split(",")
        count = 0
        cn    = {} 
        for string in sl:
            ns = string.strip()
            cn[ns] = count
            count += 1
        return cn


def read_residual_file(filename):
    f = open(filename, 'r')
    i = 0
    multi =[]
    _mat_f=[]
    for i, line in enumerate(f):
        line.strip()
        if line == "\n":
            multi.append(_mat_f)
            _mat_f = []
        else:
            lines = line.split(',')
            arr_num=[];
            for numstr in lines:
                numstr.strip()
                arr_num.append(float(numstr))
                _mat_f.append(arr_num)
    return multi

# def test():
#     print("====== start test =====")
#     pd = PointData("./phi_0_0.0000e+00.txt") 
#     print(pd.column("VALUE"))
    
#     print("====== end test =====")

# if __name__ == '__main__':
#     test()
#     pass    