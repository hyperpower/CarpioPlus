import os
import shutil
import time

def parse_name(name):
    sn = name.split("-")
    return sn

def case_info(path, origianl_files):
    info_dict = {}
    # folder name is case name
    info_dict["folder_name"] = os.path.basename(path)
    parsed_name = parse_name(info_dict["folder_name"])
    info_dict["level1"] = int(parsed_name[0])
    info_dict["level2"] = int(parsed_name[1])
    info_dict["level3"] = int(parsed_name[2])
    info_dict["name"]   = parsed_name[3]

    return info_dict

def clean(path, original_files):
    print("clean ====== ")
    files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]
    dfs   = []
    for f in files:
        if f not in original_files:
            dfs.append(f)
    for df in dfs:
        print("Remove file -> ", df)
        os.remove(df)

    dirs = [f for f in os.listdir(path) if os.path.isdir(os.path.join(path, f))]
    for d in dirs:
        print("Remove dir -> ", d)
        shutil.rmtree(d)

