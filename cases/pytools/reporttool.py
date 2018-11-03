import os
from jinja2 import Environment, Template 
import datetime

def filehead():
	text = """============================
{{ name }}
============================

"""
	return text


def filetail():
	text = """
Case Information
===================
Run time infomation:

.. table:: Summary of Running Time
   :widths: auto

   ============= =====================
    Functions      CPU Time            
   ============= =====================
   {% for rt in runtime -%}
   {{ '%12s' % rt[0] }}   {{'%15e' % rt[1]}}  
   {% endfor -%}
   ============= =====================

Files in this case:

+---------------+---------------------------+---------------------------+ 
| File          | Creat Time                | Modified Time             | 
+===============+===========================+===========================+ 
{% for file in files -%}
|{{ '%15s' % file.name }}| {{'%24s' % file.ctime}}| {{ '%24s' %  file.mtime}}| 
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
        fdict["ctime"] = datetime.datetime.fromtimestamp(info.st_ctime)
        fdict["mtime"] = datetime.datetime.fromtimestamp(info.st_mtime)
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


# get file status
# (mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime) = os.stat(file)

