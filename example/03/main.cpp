#include <iostream>

// 引入自定义头文件
#include "mclog.h"

int main(int argc, char **argv)
{
    // 打印日志
    MCLOG("hello world!")
    
    // 打印带参数格式化日志
    MCLOG($("hello world!") $(argc))
    return 0;
}