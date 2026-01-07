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


# 写入json对象到文件
def write_json_file(data, file_path):
    with open(file_path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=False)


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

    # 替换tasks文件
    file_tasks = "../.vscode/tasks.json"
    js_tasks = read_json_file(file_tasks)

    val_tasks = js_tasks["tasks"][0]
    js_tasks["tasks"].clear()
    js_tasks["tasks"].append(val_tasks)

    for i, val in enumerate(ls_dirs):
        tm = copy.deepcopy(val_tasks)
        tm["label"] = tm["label"].replace(rp_index, val)
        tm["command"] = tm["command"].replace(rp_index, val)
        tm["options"]["cwd"] = tm["options"]["cwd"].replace(rp_index, val)
        js_tasks["tasks"].append(tm)

    # 替换launch文件
    file_launch = "../.vscode/launch.json"
    js_launch = read_json_file(file_launch)

    val_launch = js_launch["configurations"][0]
    js_launch["configurations"].clear()
    js_launch["configurations"].append(val_launch)

    for i, val in enumerate(ls_dirs):
        tm = copy.deepcopy(val_launch)
        tm["name"] = tm["name"].replace(rp_index, val)
        tm["program"] = tm["program"].replace(rp_index, val)
        tm["cwd"] = tm["cwd"].replace(rp_index, val)
        tm["preLaunchTask"] = tm["preLaunchTask"].replace(rp_index, val)
        js_launch["configurations"].append(tm)

    # 保存到文件
    write_json_file(js_tasks, file_tasks)
    write_json_file(js_launch, file_launch)

    print(json.dumps(js_tasks, indent=4, ensure_ascii=False))
    print(json.dumps(js_launch, indent=4, ensure_ascii=False))

    print("< 更新完成 >")
