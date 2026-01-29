# 更新说明文档目录

import os
from pathlib import Path
import re


# 读取文件
def read_file_lines(file_path):
    ls_lines = []
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            ls_lines = f.readlines()
    except FileNotFoundError:
        print("< 读取失败 >")
    return ls_lines


# 发现文本行
def find_lines_index(ls_lines, word):
    for i, ctx in enumerate(ls_lines):
        if ctx.find(word) != -1:
            return i
    return -1


# 添加到超连接
def make_md_links(ls_lines):
    ls_links = []
    for filename in ls_lines:
        path = Path(filename)
        if path.name != "模板.md":
            link = f"[{path.stem}](./doc/{path.name})  \n"
            ls_links.append(link)
    return ls_links


# 主函数
if __name__ == "__main__":
    print("< 更新说明文档目录 >")

    # 切换到文件位置
    script_dir = Path(__file__).resolve().parent
    os.chdir(script_dir)

    # 获取文件
    path_doc = "../doc"
    file_path = "../README.md"
    word_begin = "#### 目录开始"
    word_end = "#### 目录结束"
    ls_lines = read_file_lines(file_path)
    if len(ls_lines) > 0:
        # 查找段落
        ibegin = find_lines_index(ls_lines, word_begin)
        iend = find_lines_index(ls_lines, word_end)

        # 获取文档名
        ls_names = make_md_links(os.listdir(path_doc))
        ls_names.sort(key=lambda x: int(re.search(r"\d+", x).group()))
        ls_names.append("  \n")
        ls_lines[ibegin + 1 : iend] = ls_names
        print("\n".join(ls_names))

        # 写入文件
        with open(file_path, "w", encoding="utf-8") as f:
            f.writelines(ls_lines)
            f.close()
            print("< 更新完成 >")
    else:
        print("< 更新失败 >")
