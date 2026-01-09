#ifndef BYTE_H
#define BYTE_H

// STL头文件 - 引入字符串功能 - string
#include <string>

// STL头文件 - 处理内存地址与字符 - memcpy
#include <cstring>

// 打印整数二进制函数，调用函数必须要参数，返回值可以选择性接收
// 参数为 int 类型，byte 是参数名称
// 返回值为 string 类型
std::string print_byte_int32(int byte);

// 打印浮点二进制函数
// 同理，参数 int 类型，返回值为 string 类型
std::string print_byte_float32(float byte);

#endif // BYTE_H
