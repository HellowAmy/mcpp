#ifndef REPLACE_ARGS_H
#define REPLACE_ARGS_H

#include <iostream>
#include "format.h"

// 可以接收可变数据的字符串替换格式化类
struct replace_args
{
    // 通过构造哈数，赋值一份字符串到 _org 变量
    replace_args(const std::string &org) : _org(org) {}

    // replace 无参数版本
    // 处理完成之后，最终的数据在 _org 中，返回结果
    // 这个函数只调用一次
    std::string replace()
    {
        return _org;
    }

    // replace 有参数版本
    // 可变参数模板函数 Targ 代表数量未知的
    // str_old str_new 这两个参数会从 arg 这个未知数量上拿走两个数据 arg 的数量 -2
    // 这个函数可能调用多次
    template <typename... Targ>
    std::string replace(const std::string &str_old, const std::string &str_new, Targ... arg)
    {
        // 调用 format.h replace_str_ref 来处理数据
        replace_str_ref(_org, str_old, str_new);

        // 递归调用
        // 如果 arg 的数量大于0，调用 replace 有参数版本
        // arg 如果这一次数量为0，调用 replace 无参数版本
        return replace(arg...);
    }

    // 重载 () 括号操作符，自定义返回 std::string 类型
    // 并调用 replace 函数，根据 arg 的数量选择不同版本
    // 这个函数只调用一次
    template <typename... Targ>
    std::string operator()(Targ... arg)
    {
        return replace(arg...);
    }

    // 储存传入数据的复制版本
    std::string _org;
};

#endif // REPLACE_ARGS_H
