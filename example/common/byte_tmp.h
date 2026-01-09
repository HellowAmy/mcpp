#ifndef BYTE_TMP_H
#define BYTE_TMP_H

#include <string>
#include <cstring>
#include <type_traits>

// 整形处理函数
// 模板类 print_byte 支持对更多类型
// 功能与 print_byte_int32 函数类似，可直接参考
template <typename T>
std::string print_byte(T byte, std::true_type)
{
    // 记录二进制字符串
    std::string ret;
    for (size_t i = 0; i < sizeof(T) * 8; i++)
    {
        if (byte & 0x01)
        {
            ret.push_back('1');
        }
        else
        {
            ret.push_back('0');
        }
        byte >>= 1;
    }
    return std::string(ret.rbegin(), ret.rend());
}

// 浮点类型判断函数
template <typename T>
std::string print_byte(T byte, std::false_type)
{
    // 判断浮点类型，如果符合类型拷贝内容到整数类型，并调用上方的 整形处理函数 
    // memcpy 函数可以完整复制浮点数的内存到整形
    if (std::is_same<T, float>::value)
    {
        // 4byte 浮点数
        int ibyte = 0;
        std::memcpy(&ibyte, &byte, sizeof(ibyte));
        return print_byte<int>(ibyte, std::true_type());
    }
    else if (std::is_same<T, double>::value)
    {
        // 8byte 浮点数
        long long ibyte = 0;
        std::memcpy(&ibyte, &byte, sizeof(ibyte));
        return print_byte<long long>(ibyte, std::true_type());
    }

    // 如果不是浮点类型直接返回错误结果
    return "NULL";
}

// 统一调用接口
// std::is_integral<T>() 判断类型，如果是整数返回 true_type 否则返回 false_type
// 通过 is_integral 的返回值可以进入上方的 整形处理函数 或者 浮点类型判断函数 两个函数之一
template <typename T>
std::string print_byte(T byte)
{
    return print_byte<T>(byte, std::is_integral<T>());
}

#endif // BYTE_TMP_H
