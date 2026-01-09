#include <iostream>

#include "mclog.h"

// 代码中只需包含 .h 头文件，无需关心 .cpp 源文件
#include "byte.h"

int main(int argc, char **argv)
{
    // 本月奶茶消费记录
    int pay_date_1 = 15 + 21 + 33 + 27;
    int pay_date_7 = 27 + 15 + 11;
    int pay_date_28 = 4 + 4;
    int pay_sum = pay_date_1 + pay_date_7 + pay_date_28;

    // 查看 pay 的二进制存储结构
    // 调用 print_byte_int32 ，该函数需要传入一个 int 参数，返回一个 string 类型
    std::string bit_pay_1 = print_byte_int32(pay_date_1);
    std::string bit_pay_7 = print_byte_int32(pay_date_7);
    std::string bit_pay_28 = print_byte_int32(pay_date_28);
    std::string bit_pay_sum = print_byte_int32(pay_sum);

    // 获取到 print_byte_int32 的返回结果 bit_pay ，打印这个字符串结果
    MCLOG("本月奶茶消费记录");
    MCLOG($(pay_date_1) $(bit_pay_1));
    MCLOG($(pay_date_7) $(bit_pay_7));
    MCLOG($(pay_date_28) $(bit_pay_28));
    MCLOG($(pay_sum) $(bit_pay_sum));

    return 0;
}