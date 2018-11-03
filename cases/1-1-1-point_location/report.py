import os
import sys
import run

PATH_DOC       = os.path.abspath(os.path.join(run.PATH_PROJECT, "doc"))
PATH_RSTSOURCE = os.path.abspath(os.path.join(PATH_DOC, "source"))

sys.path.append(run.PATH_PYTOOLS)
import runtool as RT 
import reporttool as REPORT 

def main():
	tmp = REPORT.filehead()

	file = open("report.rstt", "r")
	tmp += file.read()

	tmp += REPORT.filetail()

	data = RT.case_info(run.PATH_THIS, run.FILE_ORIGINAL)
	print(data)
	res = REPORT.render("res.rst", tmp, data)
	print(res)
	# get file status
# (mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime) = os.stat(file)
	print("Run report.py")

if __name__ == '__main__':
	main()