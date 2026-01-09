#ifndef BYTE_H
#define BYTE_H

// 引入字符串的STL头文件 - string
#include <string>

// 处理内存与字符的STL头文件 - memcpy
#include <cstring>

// 整数打印二进制函数
std::string print_byte_int32(int byte)
{
    // 准备字符串结果
    std::string ret;

    // 1byte 等于 8bit，循环次数为 sizeof(byte) * 8，字节数乘8
    // byte的类型是int，4byte，4*8=32,这个循环会执行32次，
    for (int i = 0; i < sizeof(byte) * 8; i++)
    {
        // 根据第一位是1，加入1，第一位是0，加入0
        if (byte & 0x01)
        {
            ret.push_back('1');
        }
        else
        {
            ret.push_back('0');
        }

        // 将第二位推到第一位，第一位消失
        byte >>= 1;
    }

    // 反转字符串顺序
    ret = std::string(ret.rbegin(), ret.rend());

    // 返回字符串结果
    return ret;
}

// 浮点数打印二进制函数
std::string print_byte_float32(float byte)
{
    // 使用 memcpy 将浮点数转移到整数类型
    int ibyte = 0;
    std::memcpy(&ibyte, &byte, sizeof(ibyte));

    // 调用整数打印二进制函数
    return print_byte_int32(ibyte);
}

#endif // BYTE_H
