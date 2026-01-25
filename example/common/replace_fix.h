
#ifndef REPLACE_FIX_H
#define REPLACE_FIX_H

#include <string>

// 替换字符串，只替换固定形式的字符串，接收多类型，并自动换行成字符串
struct replace_fix
{
    replace_fix(const std::string &org) : _org(org) {}

    // 自动将其他类型转为字符串
    template <typename T>
    std::string to_str(const T &val)
    {
        return std::to_string(val);
    }

    std::string to_str(const std::string &str)
    {
        return str;
    }

    std::string to_str(const char *str)
    {
        return str;
    }

    // 替换函数
    std::string replace_flag(const std::string &org, const std::string &str)
    {
        std::string ret;
        size_t follow = 0;
        size_t go = org.find(_flag);
        if (go == std::string::npos)
        {
            return ret;
        }
        ret += std::string(org.begin() + follow, org.begin() + go);
        ret += str;
        go += _flag.size();
        follow = go;
        ret += std::string(org.begin() + follow, org.end());
        return ret;
    }

    // 递归展开
    std::string replace()
    {
        return _org;
    }

    template <typename T,typename... Targ>
    std::string replace(T value, Targ... arg)
    {
        std::string str = to_str(value);
        _org = replace_flag(_org, str);
        return replace(arg...);
    }

    template <typename... Targ>
    std::string operator()(Targ... arg)
    {
        return replace(arg...);
    }

    std::string _org;
    std::string _flag = "{}";
};

#endif // REPLACE_FIX_H
