import os
from jinja2 import Environment, Template 


def filehead():
	text = """===================
{{ name }}
===================

"""
	return text

def filetail():
	text = """
Case information
===================
Run time infomation:

Files in this case:

	       """
	return text

def render(filename, template, renderdata):
    # Create the jinja2 environment.
    # Notice the use of trim_blocks, which greatly helps control whitespace.
    t = Template(template)
    return t.render(
    	renderdata
    )


# get file status
# (mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime) = os.stat(file)

