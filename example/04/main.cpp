#include <iostream>

#include "mclog.h"
#include "byte.h"

int main(int argc, char **argv)
{
    // 计算今日花销
    int pay = 15 + 2 + 45 + 230;
    MCLOG("计算今日花销 " << $(pay))

    // 计算零花钱
    int father = 25;
    int mother = 200;
    int nuncle_2 = 15;
    int nuncle_3 = 88;
    int money = father + mother + nuncle_2 + nuncle_3;
    int money_save = money - 288;
    MCLOG("\n计算零花钱 " << $(money))
    MCLOG("\n被妈妈保存后的剩余零花钱 " << $(money_save))

    // 计算红包面积
    double width = 12.4;
    double length = 5.8;
    double area = width * length;
    MCLOG("\n计算红包面积 " << $(area))

    // 计算兄弟平分压岁钱-你的部分
    int sum_money = 9;
    int brothers = 2;
    int part = sum_money / brothers;
    MCLOG("\n计算兄弟平分压岁钱-你的部分 " << $(part))

    // 计算兄弟平分压岁钱-重新计算
    double re_sum_money = 9.0;
    double re_brothers = 2.0;
    double re_part = re_sum_money / re_brothers;
    MCLOG("\n计算兄弟平分压岁钱-重新计算 " << $(re_part))

    // 类型字节长度
    char int8 = 0;
    short int16 = 0;
    int int32 = 0;
    long long int64 = 0;
    float f32 = 0.0;
    double f64 = 0.0;

    MCLOG("\n类型字节长度")
    MCLOG($(sizeof(int8)))
    MCLOG($(sizeof(int16)))
    MCLOG($(sizeof(int32)))
    MCLOG($(sizeof(int64)))
    MCLOG($(sizeof(f32)))
    MCLOG($(sizeof(f64)))

    MCLOG("\n正负数类型最大长度")
    MCLOG($(INT8_MAX))
    MCLOG($(INT16_MAX))
    MCLOG($(INT32_MAX))
    MCLOG($(INT64_MAX))

    MCLOG("\n正整数各类型最大长度")
    MCLOG($(UINT8_MAX))
    MCLOG($(UINT16_MAX))
    MCLOG($(UINT32_MAX))
    MCLOG($(UINT64_MAX))

    /*
        正负数与正整数的最大值：
        下面的三个变量的值，int 需要首位 0 区分正负值
        0b01111111111111111111111111111111
        0b11111111111111111111111111111111
        0b11111111111111111111111111111111
    */
    int int32_max = 0b01111111111111111111111111111111;
    int int32_max_neg = 0b11111111111111111111111111111111;
    unsigned int uint32_max = 0b11111111111111111111111111111111;
    MCLOG("\n正负数与正整数的最大值")
    MCLOG($(int32_max))
    MCLOG($(int32_max_neg))
    MCLOG($(uint32_max))

    // 正数与负数
    int pos = 5;
    int neg = -5;
    std::string bit_pos = print_byte_int32(pos);
    std::string bit_neg = print_byte_int32(neg);
    MCLOG("\n正数 " << $(bit_pos))
    MCLOG("\n负数 " << $(bit_neg))

    // 整数与浮点二进制存储格式对比
    int iyear = 12;
    float fyear = 12.0;
    std::string bit_iyear = print_byte_int32(iyear);
    std::string bit_fyear = print_byte_float32(fyear);
    MCLOG("\n整数二进制 " << $(bit_iyear))
    MCLOG("\n浮点二进制 " << $(bit_fyear))

    // 额外的测试-下面提供可口算的二进制，请分析他们的值
    /*
        整数分析：
        1.高位第一位是0,是正数
        2.采用 1248 算法分析 0100 1100
            0   1  0  0  1 1 0 0
            128 64 32 16 8 4 2 1
        3.计算出公式 4 + 8 + 64 = 76
    */
    // 直接复制二进制bit数据
    int itest = 0b00000000000000000000000001001100;

    /*
        负数分析：
        1.高位第一位是1,是负数
        2.反转 1110 1011 为 0001 0100
        3.采用 1248 算法分析 0001 0100
            0   0  0  1  0 1 0 0
            128 64 32 16 8 4 2 1
        4.计算出公式 16 + 4 = 20
        5.结果加一后取反 -1 * (20 + 1) = -21
    */
    int itest_neg = 0b11111111111111111111111111101011;

    /*
        浮点分析：
        1.先将很分段为 符号|指数|尾数 的 1|8|23 结构，0 10000010 10011000000000000000000
        2.判断为正数，指数为 10000010，使用 1248 法分析，忽略头部后结果 + 1
            0 0 1 0
            8 4 2 1
            2 + 1 = 3
        3.获取指数为 2 ^ 3 = 8，
        4.继续分析尾数，尾数计算为逐步减半法，取得总数后 + 1
            1001 1000
            1 = 0.5
            0 = 0.25
            0 = 0.125
            1 = 0.0625
            1 = 0.03125
        5.尾数公式为 1 + 0.5 + 0.0625 + 0.03125 = 1.5937
        6.合并指数和尾数公式为 1 * 1.5937 * 8 = 12.75
    */
    // 二进制只能赋值给整数，通过 memcpy 进行内存拷贝，使它们的二进制数据一致
    int iftest = 0b01000001010011000000000000000000;
    float ftest = 0.0;
    std::memcpy(&ftest, &iftest, sizeof(ftest));

    MCLOG("\n验证结果")
    MCLOG($(itest))
    MCLOG($(itest_neg))
    MCLOG($(ftest))

    return 0;
}