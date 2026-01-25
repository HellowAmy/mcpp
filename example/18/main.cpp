#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <memory>

#include "mclog.h"
#include "rand_int.h"
#include "replace_fix.h"

// 输入状态
enum class en_mode_input
{
    e_game,
    e_quit,
    e_back,
};

// 游戏状态
enum class en_mode_game
{
    e_win,
    e_failed,
    e_next,
};

// 记录输入状态，如果为游戏输入需要数字
struct input_ctx
{
    en_mode_input status = en_mode_input::e_game;
    int value = 0;
};

// 接收输入，统一获取输入状态
class recv_input
{
public:
    recv_input()
    {
        // 动态插入输入状态，提供定制输入状态
        _mp_opt_tab["quit"] = en_mode_input::e_quit;
        _mp_opt_tab["back"] = en_mode_input::e_back;
    }

    // 处理输入字符
    input_ctx get_input()
    {
        input_ctx ret;
        std::string buff;
        while (true)
        {
            std::cin >> buff;

            // 找到指定输入后返回
            auto it = _mp_opt_tab.find(buff);
            if (it != _mp_opt_tab.end())
            {
                ret.status = it->second;
                return ret;
            }

            // 这里表示启动游戏后的输入，需要保存为数字
            try
            {
                ret.value = std::stoi(buff);
                return ret;
            }
            catch (...)
            {
                print_input_tips();
            }
        }
    }

    // 更方便的状态判断函数
    bool is_game(const input_ctx &input)
    {
        return input.status == en_mode_input::e_game;
    }
    bool is_quit(const input_ctx &input)
    {
        return input.status == en_mode_input::e_quit;
    }

private:
    // 提示可输入内容
    void print_input_tips()
    {
        std::string opt;
        for (auto &a : _mp_opt_tab)
        {
            opt += "|:" + a.first + "  ";
        }
        std::string str = "输入错误，可选的值有\n{}";
        str = replace_fix(str)(opt);
        std::cout << str << std::endl;
    }

private:
    // 保存输入状态，使用 map 可动态插入新状态，方便扩展
    std::map<std::string, en_mode_input> _mp_opt_tab;
};

// 一个批量生产不同计算规则的游戏基类
// 这个类规定了运算游戏的流程，但是不会进行具体的运算
class game_flow
{
public:
    // 子类需要提供的运行数据
    struct data
    {
        int total = 0;
        int residue = 0;
        en_mode_game status = en_mode_game::e_next;
        std::vector<int> rule_num;
    };

public:
    // 开始游戏接口
    input_ctx start_game()
    {
        // 获取子类提供的数据，打印提示后开始
        data d = get_data();
        print_describe(d);
        for (int i = 0; i < d.rule_num.size(); i++)
        {
            // 判断是否需要终止游戏
            if (d.status != en_mode_game::e_next)
            {
                break;
            }

            // 获取随机数，打印本次输入提示
            int rule_num = d.rule_num[i];
            print_tips(i, rule_num, d);

            // 获取用户输入，查看是退出还是回答了问题
            input_ctx ctx = _input.get_input();
            if (_input.is_game(ctx) == false)
            {
                print_back(i, d);
                return ctx;
            }

            // 调用子类的 calc_rule 计算规则
            // 计算是否返回的结果是否都导致游戏失败
            // update_status 更新状态来决定是否失败
            int input_num = ctx.value;
            int sum = calc_rule(rule_num, input_num);
            int cur_residue = d.residue;
            int diff = d.total - sum;
            d.residue -= std::abs(diff);
            update_status(d, false);

            std::string str = "residue: {} = {} - {} >> {} = {} - |{}|\n";
            str = replace_fix(str)(diff, d.total, sum, d.residue, cur_residue, diff);
            std::cout << str << std::endl;
        }

        // 游戏结束，更新胜利状态并打印提示语
        // input_ctx() 默认状态是游戏正在进行
        update_status(d, true);
        print_game_over(d);
        return input_ctx();
    }

protected:
    // 不同游戏进入时的首次描述
    virtual void print_describe(const data &d) = 0;

    // 不同游戏的单次运行提示游戏输入的提示
    virtual void print_tips(int count, int num, const data &d) = 0;

    // 子类的计算结果
    virtual int calc_rule(int a, int b) = 0;

    // 子类需要提供的运行数据
    virtual data get_data() = 0;

private:
    // 根据计算余年更新状态
    void update_status(data &d, bool end)
    {
        if (d.residue <= 0)
        {
            d.status = en_mode_game::e_failed;
        }
        else if (d.residue > 0 && end == false)
        {
            d.status = en_mode_game::e_next;
        }
        else if (d.residue > 0 && end)
        {
            d.status = en_mode_game::e_win;
        }
    }

    // 游戏结束打印
    void print_game_over(const data &d)
    {
        std::string game_ret = "游戏胜利";
        if (d.status == en_mode_game::e_failed)
        {
            game_ret = "游戏失败";
        }

        std::string str = "{} total = {} residue = {}";
        str = replace_fix(str)(game_ret, d.total, d.residue);
        std::cout << str << std::endl;
    }

    // 临时退出时打印
    void print_back(int count, const data &d)
    {
        std::string str = "选择返回菜单，运行到 {} 次，当前结果 total = {} residue = {}";
        str = replace_fix(str)(count, d.total, d.residue);
        std::cout << str << std::endl;
    }

private:
    // 用户输入管理，用于统一处理输入状态
    recv_input _input;
};

// 乘法运算
// 继承最速公式游戏流，实现子类接口完成游戏，多态效果
class game_mul : public game_flow
{
protected:
    // 下方都为重写实现父类的虚函数
    void print_describe(const data &d) override
    {
        std::string str = "乘法计算，游戏规则如下\n"
                          "  存在公式 sum = a * b , 会用 total - sum = diff , \n"
                          "  差值累加 residue = residue - diff，当 residue <= 0 时失败\n"
                          "  我会随机提供 a ，你需要输入 b ，请让 sum 尽可能接近 total\n"
                          "  当前 total = {} residue = {} 共进行 {} 次\n";
        str = replace_fix(str)(d.total, d.residue, d.rule_num.size());
        std::cout << str << std::endl;
    }
    void print_tips(int count, int num, const data &d) override
    {
        std::string str = "当前为 {} 次，余量为 residue = {} \n"
                          "公式 sum = a * b ，本次 a = {}，请输入 b = ";
        str = replace_fix(str)(count + 1, d.residue, num);
        std::cout << str << std::endl;
    }
    int calc_rule(int a, int b) override
    {
        int sum = a * b;
        std::string str = "{} = {} * {}";
        str = replace_fix(str)(sum, a, b);
        std::cout << str << std::endl;
        return sum;
    }
    data get_data() override
    {
        data d;
        d.total = 200;
        d.residue = 100;

        // 提供随机数为计算元素，让玩家更难计算
        rand_int rand(2, 40);
        for (int i = 0; i < 5; i++)
        {
            d.rule_num.push_back(rand.value());
        }
        return d;
    }
};

// 指数运算
class game_exp : public game_flow
{
protected:
    void print_describe(const data &d) override
    {
        std::string str = "指数计算，游戏规则如下\n"
                          "  存在公式 sum = a ^ b , 会用 total - sum = diff , \n"
                          "  差值累加 residue = residue - diff，当 residue <= 0 时失败\n"
                          "  我会随机提供 a ，你需要输入 b ，请让 sum 尽可能接近 total\n"
                          "  当前 total = {} residue = {} 共进行 {} 次\n";
        str = replace_fix(str)(d.total, d.residue, d.rule_num.size());
        std::cout << str << std::endl;
    }
    void print_tips(int count, int num, const data &d) override
    {
        std::string str = "当前为 {} 次，余量为 residue = {} \n"
                          "公式 sum = a ^ b ，本次 a = {}，请输入 b = ";
        str = replace_fix(str)(count + 1, d.residue, num);
        std::cout << str << std::endl;
    }
    int calc_rule(int a, int b) override
    {
        int sum = std::pow(a, b);
        std::string str = "{} = {} ^ {}";
        str = replace_fix(str)(sum, a, b);
        std::cout << str << std::endl;
        return sum;
    }
    data get_data() override
    {
        data d;
        d.total = 1000;
        d.residue = 400;

        rand_int rand(3, 7);
        for (int i = 0; i < 3; i++)
        {
            d.rule_num.push_back(rand.value());
        }
        return d;
    }
};

// 最速公式游戏启动接口
class fast_game
{
public:
    // 存放游戏描述和启动游戏的指针，使用智能指针管理内存
    struct game_desc
    {
        std::string desc;
        std::shared_ptr<game_flow> game;
    };

public:
    // 启动入口
    void run()
    {
        while (_run)
        {
            // 显示菜单并获取输入
            print_menu();
            input_ctx input = _input.get_input();

            // 判断是否开始游戏
            if (_input.is_game(input))
            {
                // 找到指定游戏并运行
                game_flow *game = game_type(input.value);
                if (game)
                {
                    input = game->start_game();
                }
            }

            // 判断是否退出，包括游戏中的状态返回
            if (_input.is_quit(input))
            {
                print_quit();
                break;
            }
        }
    }

    // 安装游戏类型
    void install_game(const game_desc &game)
    {
        _mp_game[_mp_game.size() + 1] = game;
    }

private:
    // 显示菜单
    void print_menu()
    {
        std::string opt;
        for (auto &a : _mp_game)
        {
            opt += replace_fix("{}.{} \n")(a.first, a.second.desc);
        }
        std::string str = "\n欢迎来到最速公式，可选择以下游戏\n{}";
        str = replace_fix(str)(opt);
        std::cout << str << std::endl;
    }

    // 结束游戏
    void print_quit()
    {
        std::cout << "欢迎下次游玩" << std::endl;
    }

    // 查找到指定游戏后返回
    game_flow *game_type(int index)
    {
        auto it = _mp_game.find(index);
        if (it != _mp_game.end())
        {
            return it->second.game.get();
        }
        return nullptr;
    }

private:
    bool _run = true;
    recv_input _input;

    // 使用 map 容器可以动态的扩展选项
    std::map<int, game_desc> _mp_game;
};

int main(int argc, char **argv)
{
    fast_game game;

    {
        // 动态添加游戏
        // 智能指针管理权会将转移到 fast_game 中的 map 容器中
        fast_game::game_desc d;
        d.desc = "乘法游戏";
        d.game = std::make_shared<game_mul>();
        game.install_game(d);
    }
    {
        fast_game::game_desc d;
        d.desc = "指数游戏";
        d.game = std::make_shared<game_exp>();
        game.install_game(d);
    }

    game.run();

    return 0;
}
