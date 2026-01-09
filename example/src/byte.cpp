#include "byte.h"

// 函数解析
// std::string 是函数返回值
// print_byte_int32 是函数名称
// int byte 函数参数，int 是类型，byte 是名称
std::string print_byte_int32(int byte)
{
    // 函数作用域开始

    // 准备字符串结果
    std::string ret;

    // 循环的功能是将二进制数据循环推到第一位 bit 上进行判断
    // 1byte 等于 8bit，循环次数为 sizeof(byte) * 8，字节数乘8
    // byte的类型是int，4byte，4*8=32,这个循环会执行32次，
    // 4byte 的 bit 结构 00000000 00000000 00000000 00000000
    //                                                    ^ 这是第一位 bit 位置
    for (int i = 0; i < sizeof(byte) * 8; i++)
    {
        // 这个判断总是在判断第一位 bit 位置
        // 我判断的是 ^ 箭头上面的 bit 是 1 还是 0
        // 二进制存储顺序为 左高右低 ，第一位是又右边的 bit
        // 根据第一位是1，加入1，第一位是0，加入0
        if (byte & 0x01)
        {
            ret.push_back('1');
        }
        else
        {
            ret.push_back('0');
        }

        // 从左往右推 1bit ，第二位会覆盖第一位，第一位会消失
        // >>= 等效于 byte = byte >> 1
        byte >>= 1;
    }

    // 反转字符串顺序
    // 存入 string 的结构是 左低右高，和内存顺序 左高右低 相反，需要反转
    ret = std::string(ret.rbegin(), ret.rend());

    // 返回字符串结果，函数如果存在返回值，你必须调用 return 返回内容
    return ret;

    // 函数作用域结束
}

std::string print_byte_float32(float byte)
{
    // 使用 memcpy 将浮点数转移到整数类型
    int ibyte = 0;
    std::memcpy(&ibyte, &byte, sizeof(ibyte));

    // 调用整数打印二进制函数
    return print_byte_int32(ibyte);
}
