#include <iostream>
#include <vector>
#include <algorithm>

#include "mclog.h"

// 常量类型-数据不可重新赋值
// 定义统一的类型，统一菜单选项名称，防止输入错误
const int _input_quit_ = -1;
const int _input_back_ = -2;

namespace global_name
{
    int input_quit = -1;
};

// 替换字符串，用于格式化字符串
// 在 org 字符串内，使用 str_new 替换 str_old 字符
void replace_str_ref(std::string &org, const std::string &str_old, const std::string &str_new)
{
    /*
        描述运行逻辑：
        1.查找 str_old 的位置，如果找到则使用 go 标记位置，复制 go 与 follow 之间的内容
        2.复制之后 follow 会尾随 go 的位置并跳过 str_old 字符
        3.循环使用 go 寻找下一个 str_old 字符
        4.结尾处复制 follow 位置到字符串末端，复制收尾字符
    */
    std::string ret;
    size_t go = 0;
    size_t follow = 0;
    while (true)
    {
        go = org.find(str_old, go);
        if (go == std::string::npos)
        {
            break;
        }
        ret += std::string(org.begin() + follow, org.begin() + go);
        ret += str_new;
        go += str_old.size();
        follow = go;
    }
    ret += std::string(org.begin() + follow, org.end());
    org = ret;
}

// 打印输出错误提示
void print_input_tips()
{
    // 使用 str temp等变量名称时，通常时一个临时变量命名，无含义名称
    // \n 为打印时换行，C++允许多行字符串使用 "" 进行文本编写，实际效果为一行字符串
    // 例子
    //      "hello"
    //      "world"
    // 实际打印 "hello world"
    std::string str = "你需要输入数字，或者以下选项\n"
                      "quit: 退出游戏\n"
                      "back: 返回主菜单\n";
    std::cout << str << std::endl;
}

// 打印游戏菜单显示
void print_game_menu()
{
    std::string str = "欢迎游玩 \"最速公式\" 口算游戏，你可以选择以下游戏类型\n"
                      "1.加法公式\n"
                      "2.乘法公式\n"
                      "3.指数公式\n";
    std::cout << str << std::endl;
}

// 获取用户输入
// 返回值只返回大于零的数和 _input_quit_ _input_back_ 两个负数常量
int get_input()
{
    // 循环获取输入，直到符合返回结果
    std::string input;
    while (true)
    {
        // 从终端获取输入
        std::cin >> input;

        // 判断 input 的输入是否为菜单选项
        if (input == "quit")
        {
            return _input_quit_;
        }
        else if (input == "back")
        {
            return _input_back_;
        }

        // cin 出现错误时退出
        if (std::cin.bad())
        {
            break;
        }

        // 异常块代码，处理 stoi 的异常抛出
        try
        {
            // 将string转为int类型，并返回
            return std::stoi(input);
        }
        catch (...)
        {
            // stoi 发出异常，说明输入不为数字
            // 打印数入提示
            print_input_tips();
        }
    }

    // 返回退出类型
    return _input_quit_;
}

// 加法游戏，会显示游戏玩法，然后接收玩家输入
// 游戏会通过 vec_rule 预设值进行循环，玩家失败会提前结束循环
// 退出是会附带返回值，表明需要返回的地方
int game_add()
{
    // 游戏目标
    int total = 100;

    // 游戏预设值
    std::vector<int> vec_rule = {14, 7, 21, 17, 3};

    // 游戏提示
    std::string str_rule =
        "加法计算游戏，游戏规则如下\n"
        "让x接近{0}，我会给出a，你需要输入b，超出或者少于x的部分会让total减少\n"
        "游戏公式 x = a + b，胜利条件 total > {0}\n"
        "你一共有 {1} 次计算，请开始游戏\n";

    // 替换描述符号将 "{0}" 替换成 total 的值，结果为 "{0}" = 100
    replace_str_ref(str_rule, "{0}", std::to_string(total));
    replace_str_ref(str_rule, "{1}", std::to_string(vec_rule.size()));

    // 打印提示
    std::cout << str_rule << std::endl;

    // 有限循环函数，for 会循环 vec_rule 的数量，目前是5次，5次之后会自动退出循环
    int total_keep = total;
    for (size_t i = 0; i < vec_rule.size(); i++)
    {
        // 每次输入时提示语
        int val_a = vec_rule[i];
        std::string str_tips =
            "当前 total={0}，求公式 {1} = {2} + b\n"
            "求公式 {1} = {2} + b\n"
            "第{3}次计算，请输入b=\n";
        replace_str_ref(str_tips, "{0}", std::to_string(total));
        replace_str_ref(str_tips, "{1}", std::to_string(total_keep));
        replace_str_ref(str_tips, "{2}", std::to_string(val_a));
        replace_str_ref(str_tips, "{3}", std::to_string(i + 1));
        std::cout << str_tips << std::endl;

        // 获取玩家输入值，如果 val_b 不是负数，则说明游戏继续
        int val_b = get_input();

        // 如果输入小于0，说明用户输入了 quit back 其中的一个值会提起退出游戏
        if (val_b < 0)
        {
            return val_b;
        }

        // 正常输入了数字，计算差值公式
        int diff = total_keep - (val_a + val_b);

        // 通过 diff 差值计算 total 剩余的量，等与0时会直接失败
        total -= std::abs(diff);

        // 失败时提前退出游戏
        if (total <= 0)
        {
            break;
        }
    }

    // 判断胜利
    // if 接收一个 bool 值，结果为 true 进入if分支，否则进入else分支
    if (total > 0)
    {
        std::cout << "游戏胜利 total = " << total << std::endl;
    }
    else
    {
        std::cout << "游戏失败 total = " << total << std::endl;
    }

    // 返回到主菜单
    return _input_back_;
}

// 乘法游戏
int game_mul()
{
    std::cout << "游戏正在更新中，敬请期待" << std::endl;
    return _input_back_;
}

// 指数游戏
int game_exp()
{
    std::cout << "游戏正在更新中，敬请期待" << std::endl;
    return _input_back_;
}

// 入口函数
int main(int argc, char **argv)
{
    // 显示游戏菜单
    print_game_menu();

    // 是否继续运行标记
    bool run = true;

    // 无限循环函数，使用 run 判断是否继续循环
    // while 需要 bool 类型，为 true 时继续循环，false 时会退出循环
    while (run)
    {
        // 请求玩家输入，是进入游戏还是退出
        int input = get_input();

        // 判断游戏选择的类型
        // switch 函数会根据 input 的值，在分支号 123 中选择对应的分支，如果不存在则进入 default 分支
        // 每个分支的作用域内，需要调用 break 退出，否则 switch 会一致按顺序执行所有分支
        // 即使分支号和input无关
        switch (input)
        {
        case 1:
        {
            // 选择了加法游戏，游戏结束时会返回游戏下一步的计划
            input = game_add();
            break;
        }
        case 2:
        {
            input = game_mul();
            break;
        }
        case 3:
        {
            input = game_exp();
            break;
        }
        default:
            // 不是 123 中的数字，不存在选择类型，无事发生
            break;
        }

        // 重新判断 input 输入是否会退出游戏
        // switch 函数会根据 input 的值，在分支号 _input_back_  _input_quit_ 中选择对应的分支
        // switch 只能接收整数类型和枚举类型
        switch (input)
        {
        case _input_back_:
        {
            // 返回主菜单，打印游戏菜单
            print_game_menu();
            break;
        }
        case _input_quit_:
        {
            // 退出游戏，结束循环函数，函数会运行到尾部之后退出
            run = false;
            break;
        }
        default:
            // 不存在输入，提示输入错误
            print_input_tips();
            break;
        }
    }

    std::cout << "欢迎下次游玩" << std::endl;
    return 0;
}