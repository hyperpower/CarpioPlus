import os
import sys
import run as RUN

PATH_DOC       = os.path.abspath(os.path.join(RUN.PATH_PROJECT, "doc"))
PATH_RSTSOURCE = os.path.abspath(os.path.join(PATH_DOC, "source"))

sys.path.append(RUN.PATH_PYTOOLS)
import runtool as RT 
import reporttool as REPORT 

def copy_fig(folder_name):
	os.system("rm -r " + PATH_RSTSOURCE + "/_static/" + folder_name)
	os.system("cp -r " + RUN.PATH_FIG + " " + PATH_RSTSOURCE + "/_static/" + folder_name)

def run(runtime):
	tmp  = REPORT.filehead()

	file = open("report.rstt", "r")
	tmp += file.read()
	file.close()

	tmp += REPORT.filetail()
	
	# prepare data ===============================================
	data      = {}
	case_data = RT.case_info(RUN.PATH_THIS, RUN.FILE_ORIGINAL)
	data.update(case_data)
	file_data = REPORT.file_info(RUN.PATH_THIS, RUN.FILE_ORIGINAL)
	data["files"]   = file_data 
	data["runtime"] = runtime
	# copy fig ===================================================
	copy_fig(data["folder_name"])

	res = REPORT.render(tmp, data)

	out_file = open(PATH_RSTSOURCE + "/" + case_data["folder_name"] + ".rst", "w")
	out_file.write(res)
	out_file.close()

def main():
	run([()])

if __name__ == '__main__':
	main()