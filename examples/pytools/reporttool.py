import os
from jinja2 import Environment, Template 
import datetime
import runtool as RT


def filehead():
	text = """=================================
{{ name }}
=================================

"""
	return text


def filetail():
	text = """
Case Information
===================
Run time infomation:

.. table:: Summary of Running Time.
   :widths: auto
   :align: center 

   ============= =====================
    Functions      Wall Time (s)            
   ============= =====================
   {% for rt in runtime -%}
   {{ '%12s' % rt[0] }}   {{'%15.5e' % rt[1]}}  
   {% endfor -%}
   ============= =====================


.. table:: Files in this case.
   :widths: auto
   :align: center 

   +---------------+---------------------------+---------------------------+ 
   | File          | Creat Time                | Modified Time             | 
   +===============+===========================+===========================+ 
   {% for file in files -%}
   |{{ '%15s' % file.name }}| {{'%26s' % file.ctime}}| {{ '%26s' %  file.mtime}}| 
   +---------------+---------------------------+---------------------------+
   {% endfor %}
"""
	return text


# {% for file in files -%}
# |{{ file.name }}| {{file.ctime}}| {{file.mtime}}    | 
# +-------------+------------+----------------+
# {% endfor %}

def file_info(path, origianl_files):
    res = []
    for f in origianl_files:
        full_path = path + "/" + f
        info = os.stat(full_path)
        fdict = {}
        fdict["name"]  = f
        ctime = datetime.datetime.fromtimestamp(info.st_ctime)
        fdict["ctime"] = ctime.strftime("%y-%m-%d %H:%M")
        mtime = datetime.datetime.fromtimestamp(info.st_mtime)
        fdict["mtime"] = mtime.strftime("%y-%m-%d %H:%M")
        fdict["size"]  = info.st_size
        res.append(fdict)
    return res


def render(template, renderdata):
    # Create the jinja2 environment.
    # Notice the use of trim_blocks, which greatly helps control whitespace.
    t = Template(template)
    return t.render(
    	renderdata
    )


def copy_fig_folder(path, folder_name):
    # check before copy
    if not os.path.isdir(path.rstsource + "/_static"):
      os.system("mkdir "+ path.rstsource + "/_static")
    if os.path.isdir(path.rstsource + "/_static/" + folder_name):
      os.system("rm -r " + path.rstsource + "/_static/" + folder_name)
    # copy
    os.system("cp -r " + path.fig + " " + path.rstsource + "/_static/" + folder_name)

def run(path, runtime, origianl_files, data_extra = None):
    tmp  = filehead()

    file = open(path.this + "/report.rst", "r")
    tmp += file.read()
    file.close()

    tmp += filetail()

    # prepare data ===============================================
    data      = {}
    case_data = RT.case_info(path.this, origianl_files)
    data.update(case_data)
    file_data = file_info(path.this, origianl_files)
    data["files"]   = file_data 
    data["runtime"] = runtime
    if data_extra is not None:
        data.update(data_extra)
    # copy fig ===================================================
    copy_fig_folder(path, data["folder_name"])

    res = render(tmp, data)

    out_file = open(path.rstsource + "/" + case_data["folder_name"] + ".rst", "w")
    out_file.write(res)
    out_file.close()

# get file status
# (mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime) = os.stat(file)

