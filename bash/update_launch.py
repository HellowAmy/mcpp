# 更新一键启动配置

import copy
import json
import os
from pathlib import Path


# 获取路径下可启动文件夹
def get_path_launch_dirs(path):
    ls_dirs = []
    ls_it = os.listdir(path)
    for item in ls_it:
        if os.path.isdir(os.path.join(path, item)):
            if os.path.exists(os.path.join(path, item, "main.cpp")):
                ls_dirs.append(item)
    ls_dirs.sort()
    return ls_dirs


# 读取json文件到对象
def read_json_file(file_path):
    data: json
    with open(file_path, "r", encoding="utf-8") as f:
        data = json.load(f)
    return data


# 读取txt文件到列表
def read_txt_file(file_path):
    ls_content = []
    with open(file_path, "r", encoding="utf-8") as f:
        ls_content = f.readlines()
    return ls_content


# 写入json对象到文件
def write_json_file(data, file_path):
    with open(file_path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=False)


# 写入列表到文件
def write_txt_file(data, file_path):
    with open(file_path, "w", encoding="utf-8") as f:
        f.writelines(data)


# 修改tasks文件
def change_tasks(rp_index, ls_dirs):
    file_tasks = "../.vscode/tasks.json"
    js_tasks = read_json_file(file_tasks)

    val_tasks = js_tasks["tasks"][0]
    js_tasks["tasks"].clear()
    js_tasks["tasks"].append(val_tasks)

    for val in ls_dirs:
        tm = copy.deepcopy(val_tasks)
        tm["label"] = tm["label"].replace(rp_index, val)
        tm["command"] = tm["command"].replace(rp_index, val)
        tm["options"]["cwd"] = tm["options"]["cwd"].replace(rp_index, val)
        js_tasks["tasks"].append(tm)

    write_json_file(js_tasks, file_tasks)
    print(json.dumps(js_tasks, indent=4, ensure_ascii=False))


# 修改launch文件
def change_launch(rp_index, ls_dirs):
    file_launch = "../.vscode/launch.json"
    js_launch = read_json_file(file_launch)

    val_launch = js_launch["configurations"][0]
    js_launch["configurations"].clear()
    js_launch["configurations"].append(val_launch)

    for val in ls_dirs:
        tm = copy.deepcopy(val_launch)
        tm["name"] = tm["name"].replace(rp_index, val)
        tm["program"] = tm["program"].replace(rp_index, val)
        tm["cwd"] = tm["cwd"].replace(rp_index, val)
        tm["preLaunchTask"] = tm["preLaunchTask"].replace(rp_index, val)
        js_launch["configurations"].append(tm)

    write_json_file(js_launch, file_launch)
    print(json.dumps(js_launch, indent=4, ensure_ascii=False))


# 修改CMakeLists文件
def change_cmakelists(rp_index, ls_dirs):
    file_cmakelists = "../example/CMakeLists.txt"
    ls_cmakelists = read_txt_file(file_cmakelists)
    ibegin = -1
    iflg = -1
    iend = -1
    for i, val in enumerate(ls_cmakelists):
        if val.find("###") != -1:
            if ibegin == -1:
                ibegin = i
            elif iflg == -1:
                iflg = i
            else:
                iend = i
                break

    print(ibegin, iflg, iend)
    ls_change = ls_cmakelists[ibegin + 1 : iflg]
    for i, val in enumerate(ls_change):
        ls_change[i] = val.replace("# ", "")
    str_change = "".join(ls_change)

    ret = ""
    for val in ls_dirs:
        tm = copy.deepcopy(str_change)
        tm = tm.replace(rp_index, val)
        ret += tm

    ls_cmakelists[iflg + 1 : iend] = []
    ls_cmakelists.insert(iflg + 1, ret)

    write_txt_file(ls_cmakelists, file_cmakelists)
    print("".join(ls_cmakelists))


# 主函数
if __name__ == "__main__":
    print("< 更新一键启动配置 >")

    # 切换到文件位置
    script_dir = Path(__file__).resolve().parent
    os.chdir(script_dir)

    #
    rp_index = "#INDEX#"
    path_example = "../example"
    ls_dirs = get_path_launch_dirs(path_example)

    # 保存到文件
    change_tasks(rp_index, ls_dirs)
    change_launch(rp_index, ls_dirs)
    change_cmakelists(rp_index, ls_dirs)

    print("< 更新完成 >")
