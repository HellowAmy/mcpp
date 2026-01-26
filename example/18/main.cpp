#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <memory>
#include <functional>
#include <atomic>

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
        // 接收输入，直到符合需求后退出
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
        // 记录游戏开始
        using namespace std::chrono;
        _game_time = 0;
        auto begin_time = steady_clock::now();

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

            // 显示计算后余量
            std::string str = "residue: {} = {} - {} >> {} = {} - |{}|\n";
            str = replace_fix(str)(diff, d.total, sum, d.residue, cur_residue, diff);
            std::cout << str << std::endl;
        }

        // 游戏结束，更新胜利状态并打印提示语
        // input_ctx() 默认状态是游戏正在进行
        update_status(d, true);
        print_game_over(d);

        // 计算游戏时间，并记录最快时间
        auto end_time = steady_clock::now();
        _game_time = duration_cast<seconds>(end_time - begin_time).count();
        if (_fast_game_time == 0 || _game_time < _fast_game_time)
        {
            if (d.status == en_mode_game::e_win)
            {
                _fast_game_time = _game_time;
            }
        }
        return input_ctx();
    }

    // 获取完成游戏的时间
    size_t finish_game_time_sec()
    {
        return _game_time;
    }

    // 最快通过时间
    size_t fast_game_time_sec()
    {
        return _fast_game_time;
    }

    // 游戏名称
    virtual std::string game_name() = 0;

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

    // 记录通过时间和最快通过时间
    size_t _game_time = 0;
    size_t _fast_game_time = 0;
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
                          "公式 {} = {} * b ，请输入 b = ";
        str = replace_fix(str)(count + 1, d.residue, d.total, num);
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
        for (int i = 0; i < 3; i++)
        {
            d.rule_num.push_back(rand.value());
        }
        return d;
    }
    std::string game_name() override
    {
        return "乘法计算";
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
                          "公式 {} = {} ^ b ，请输入 b = ";
        str = replace_fix(str)(count + 1, d.residue, d.total, num);
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
        d.residue = 500;

        rand_int rand(3, 7);
        for (int i = 0; i < 3; i++)
        {
            d.rule_num.push_back(rand.value());
        }
        return d;
    }
    std::string game_name() override
    {
        return "指数计算";
    }
};

// 最速公式游戏启动接口
class fast_game
{
public:
    fast_game()
    {
        // 使用 shared_ptr 智能指针管理线程，这里提供了创建和销毁方式
        // new std::thread(&fast_game::work_countdown_th, this) 创建一个线程并运行
        // th->join(); 回调是用于销毁线程
        _th_countdown = std::shared_ptr<std::thread>(
            new std::thread(&fast_game::work_countdown_th, this),
            [](std::thread *th)
            { th->join(); });
    }

    // 倒计时线程
    void work_countdown_th()
    {
        _countdown = -1;
        while (_run)
        {
            // 写入倒计时内容，使用 flush 函数刷新缓冲区
            if (_countdown >= 0)
            {
                std::cout << "\033[s";
                std::cout << "\033[3A";
                std::cout << "\r" << "countdown: " << _countdown;
                std::cout << "\033[u";
                std::flush(std::cout);
                _countdown++;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

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
                    start_game();
                    input = game->start_game();
                    end_end();

                    // 打印游戏时间
                    size_t sec = game->finish_game_time_sec();
                    if (sec > 0)
                    {
                        int fast_time = game->fast_game_time_sec();

                        std::string str = "本次游戏时间为: {}秒\n"
                                          "最快记录为: {}秒\n";
                        str = replace_fix(str)(sec, fast_time);
                        std::cout << str << std::endl;
                    }
                }
            }

            // 判断是否退出，包括游戏中的状态返回
            if (_input.is_quit(input))
            {
                print_quit();
                _run = false;
            }
        }
    }

    // 安装游戏类型
    void install_game(std::shared_ptr<game_flow> game)
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
            opt += replace_fix("{}.{} \n")(a.first, a.second->game_name());
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
            return it->second.get();
        }
        return nullptr;
    }

    // 开始游戏时初始化参数
    void start_game()
    {
        _countdown = 0;
    }

    // 结束游戏时回收
    void end_end()
    {
        _countdown = -1;
    }

private:
    bool _run = true;
    recv_input _input;

    // 使用 map 容器可以动态的扩展选项
    // 存放启动游戏的指针，使用智能指针管理内存
    std::map<int, std::shared_ptr<game_flow>> _mp_game;

    // 原子变量，保证多线程修改数据准确
    std::atomic<int> _countdown;

    // 倒计时线程，在构造函数中初始化
    std::shared_ptr<std::thread> _th_countdown = nullptr;
};

int main(int argc, char **argv)
{
    fast_game game;

    // 动态添加游戏
    // 智能指针管理权会将转移到 fast_game 中的 map 容器中
    game.install_game(std::make_shared<game_mul>());
    game.install_game(std::make_shared<game_exp>());

    game.run();

    return 0;
}
