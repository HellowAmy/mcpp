/*
    这是多行注释，在标记结束前都是注释内容，可以有很多行
    多行注释主要用于集中描述复杂功能，或者给开发者提供帮助
    注释内容不是运行代码的一部分，仅仅是用于开发者了解程序设计
    这是一个简单的 C++ 代码文件，用于在终端中打印 hello world
    编写注释是一个很好的习惯，请保持编写你的设计思路
    正如你现在所看到的，现在这一份代码充满了注释
*/

// 这是单行注释，单行注释用于在注释下方，一行或连续多行的代码功能
// 这是包含标准库（STL）头文件的代码
#include <iostream>

// 宏定义（macro）：
// 一个用于打印到终端的宏
#define MCLOG(value) std::cout << value << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl;

// 一个用于设置参数的宏
#define $(value) "[" #value ": " << value << "] "

// main 函数入口，一个可执行程序只能有存在唯一的一个 main 函数
// main 函数存在返回值 int 和两个传入参数 int 与 char **
// argc : 传入的参数个数
// argv : 存储的字符数据
int main(int argc, char **argv)
{
    // 打印启动参数
    std::cout << "argv: ";
    for (int i = 0; i < argc; i++)
    {
        char *val = argv[i];
        std::cout << val;
        std::cout << " ";
    }
    std::cout << std::endl;

    // C++ 常规打印
    std::cout << "hello world" << std::endl;

    // C++ 带定位的打印
    std::cout << "hello world" << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl;

    // C++ 带定位的宏打印
    MCLOG("hello world")

    // C++ 带定位和参数格式化的宏打印
    MCLOG($("hello world") $(argc))

    // main 函数返回值，通常 0 代表正常返回，负数为异常终止
    return 0;
}