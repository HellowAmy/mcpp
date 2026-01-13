#include "format.h"

void replace_str_ref(std::string &org, const std::string &str_old, const std::string &str_new)
{
    /*
        描述运行逻辑：
        1.查找 str_old 的位置，如果找到则使用 go 标记位置，复制 go 与 follow 之间的内容
        2.复制之后 follow 会尾随 go 的位置并跳过 str_old 字符
        3.循环使用 go 寻找下一个 str_old 字符
        4.结尾处复制 follow 位置到字符串末端，复制收尾字符
    */
    std::string ret;
    size_t go = 0;
    size_t follow = 0;
    while (true)
    {
        go = org.find(str_old, go);
        if (go == std::string::npos)
        {
            break;
        }
        ret += std::string(org.begin() + follow, org.begin() + go);
        ret += str_new;
        go += str_old.size();
        follow = go;
    }
    ret += std::string(org.begin() + follow, org.end());
    org = ret;
}