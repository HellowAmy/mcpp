#include <iostream>

#include "mclog.h"
#include "byte_tmp.h"

int main(int argc, char **argv)
{
    // 类型字节长度
    char int08 = '8';
    short int16 = 16;
    int int32 = 32;
    long long int64 = 64;
    float f32 = 32.0;
    double f64 = 64.0;

    // 模板函数调用-自动匹配调用
    std::string bit_int08 = print_byte(int08);
    std::string bit_int16 = print_byte(int16);
    std::string bit_int32 = print_byte(int32);
    std::string bit_int64 = print_byte(int64);
    std::string bit_f32 = print_byte(f32);
    std::string bit_f64 = print_byte(f64);

    // 模板函数调用-指定类型调用
    /*
        std::string bit_int08 = print_byte<char>(int08);
        std::string bit_int16 = print_byte<short>(int16);
        std::string bit_int32 = print_byte<int>(int32);
        std::string bit_int64 = print_byte<long long>(int64);
        std::string bit_f32 = print_byte<float>(f32);
        std::string bit_f64 = print_byte<double>(f64);
    */

    // 假设采用普通函数调用
    /*
        std::string bit_int08 = print_byte_int8<char>(int08);
        std::string bit_int16 = print_byte_int16<short>(int16);
        std::string bit_int32 = print_byte_int32<int>(int32);
        std::string bit_int64 = print_byte_int64<long long>(int64);
        std::string bit_f32 = print_byte_float32<float>(f32);
        std::string bit_f64 = print_byte_float64<double>(f64);
    */

    MCLOG("类型字节长度");
    MCLOG($(int08) $(bit_int08));
    MCLOG($(int16) $(bit_int16));
    MCLOG($(int32) $(bit_int32));
    MCLOG($(int64) $(bit_int64));
    MCLOG($(f32) $(bit_f32));
    MCLOG($(f64) $(bit_f64));

    return 0;
}