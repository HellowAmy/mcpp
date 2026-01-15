#include <iostream>

#include "mclog.h"
#include "replace_args.h"

int main(int argc, char **argv)
{
    // 使用 const 保持数据不变
    const std::string name = "JudyHopps";
    const std::string introduce = "我是兔朱迪和尼克狐尼克组成了动物城最强搭档";
    const std::string content = "\n这里是兔子胡萝卜节表演大会\n请 {1} 进行自我介绍\n{2}\n";

    MCLOG("使用 replace_str_ref 函数");
    std::string tm_replace_str_ref = content;
    replace_str_ref(tm_replace_str_ref, "{1}", name);
    replace_str_ref(tm_replace_str_ref, "{2}", introduce);
    MCLOG($(tm_replace_str_ref));

    MCLOG("\n使用 replace_args 对象，多次替换");
    std::string tm_replace_args = content;
    std::string ret_replace_args = replace_args(tm_replace_args)("{1}", name, "{2}", introduce);
    MCLOG($(ret_replace_args));

    MCLOG("\n使用 replace_args 对象，只替换一次");
    std::string tm_replace_args_1 = content;
    std::string ret_replace_args_1 = replace_args(tm_replace_args_1)("{1}", name);
    MCLOG($(ret_replace_args_1));

    return 0;
}