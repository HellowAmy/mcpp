#include <iostream>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <functional>

#include "mclog.h"
#include "rand_int.h"
#include "replace_fix.h"

// 对容器线程安全的简单封装
template <typename T>
class queue_th
{
public:
    void push(T &&val)
    {
        // 上锁
        std::lock_guard<std::mutex> lock(_mut);

        // 完美转发数据，在中间层转发常用方式
        _que.push(std::forward<T>(val));
    }

    std::pair<bool, T> pop_safe()
    {
        // 取判空和数据需要一气呵成，否则没有互斥的情况下会出现多次弹出的问题
        std::lock_guard<std::mutex> lock(_mut);
        if (_que.empty() == false)
        {
            T ret = _que.front();
            _que.pop();
            return std::make_pair(true, ret);
        }
        return std::make_pair(false, T());
    }

    size_t size()
    {
        return _que.size();
    }

    bool is_empty()
    {
        return _que.empty();
    }

private:
    // STL的容器是线程不安全的，在多线程使用需要给容器上锁
    std::mutex _mut;
    std::queue<T> _que;
};

// 鸡蛋组信息
struct info_eggs
{
    int tag = 0;
    int size = 0;
    size_t date = 0;
    std::vector<int> id;
};

// 鸡蛋工厂
class factory_eggs
{
public:
    factory_eggs() : _run(true)
    {
        // 销毁函数
        auto fn_delete = [](std::thread *th)
        {
            th->join();
            MCLOG("线程退出")
        };

        // 三个发货工人
        auto fn_package = std::bind(&factory_eggs::work_package_eggs, this);
        auto sp_package_1 = std::shared_ptr<std::thread>(new std::thread(fn_package), fn_delete);
        auto sp_package_2 = std::shared_ptr<std::thread>(new std::thread(fn_package), fn_delete);
        auto sp_package_3 = std::shared_ptr<std::thread>(new std::thread(fn_package), fn_delete);

        // 一个采集机器
        auto fn_collect = std::bind(&factory_eggs::work_collect_eggs, this);
        auto sp_collect_1 = std::shared_ptr<std::thread>(new std::thread(fn_collect), fn_delete);

        _works.push_back(sp_package_1);
        _works.push_back(sp_package_2);
        _works.push_back(sp_package_3);
        _works.push_back(sp_collect_1);
    }

    ~factory_eggs()
    {
        _run = false;
        _cond.notify_all();
        MCLOG("退出鸡蛋工厂 " $(_que.size()));
    }

    // 注册发货信息接收
    void register_transport(std::function<void(info_eggs)> fn)
    {
        _fn_transport = fn;
    }

private:
    // 人工打包并发货
    void work_package_eggs()
    {
        MCLOG("进入发货流程");
        while (_run || _que.size() > 0)
        {
            info_eggs ct;

            // 进入上锁区域
            {
                // 使用 while 而不是 if 判断，防止空唤醒，如果唤醒时是空可以自己休眠
                std::unique_lock<std::mutex> lock(_mut);
                while (_que.is_empty() && _run)
                {
                    _cond.wait(lock);
                }
                _tag++;
                ct.tag = _tag;
                ct.size = 6;
                ct.date = std::chrono::steady_clock::now().time_since_epoch().count();
                for (int i = 0; i < ct.size; i++)
                {
                    // 判断是否能接收满一打，接收不到就等
                    while (_que.is_empty() && _run)
                    {
                        _cond.wait(lock);
                    }

                    // 上一行 is_empty 不为空，进入到这一行之后，多线程下还是存在为空的可能
                    // 取数据需要判空和弹出一气呵成，否则不安全
                    auto pair = _que.pop_safe();
                    if (pair.first)
                    {
                        ct.id.push_back(pair.second);
                    }
                }
                // 到这会退出作用域，解锁
            }

            // 无锁区域，发货
            if (_fn_transport)
            {
                _fn_transport(ct);
            }

            // 人工发货，发货几次后休息
            static int count = 0;
            count++;
            if (count > 3)
            {
                count = 0;
                relax_time();
            }
        }
        MCLOG("退出发货流程 " $(_que.size()));
    }

    // 机器采集，全年无休
    void work_collect_eggs()
    {
        MCLOG("进入采集流程");
        while (_run)
        {
            if (_que.size() < 5000)
            {
                rand_int rand_size(1, 36);
                rand_int rand_id(0, 10000);
                for (int i = 0; i < rand_size.value(); i++)
                {
                    _que.push(rand_id.value());
                }
                _cond.notify_one();
            }
        }
        MCLOG("退出采集流程 " $(_que.size()));
    }

    // 采集之后的休息时间
    void relax_time()
    {
        // 静态变量，一个程序只声明和定义一次，直到程序总结才会销毁
        static rand_int rand_ms(10, 50);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand_ms.value()));
    }

private:
    // 分发鸡蛋时使用
    int _tag = 0;
    std::function<void(info_eggs)> _fn_transport = nullptr;

    // 控制线程数据交换时使用
    std::atomic<bool> _run;
    queue_th<int> _que;
    std::mutex _mut;

    // 条件变量，在多线程下，用于唤醒休眠的线程
    // 线程可以通过 condition_variable 控制休眠和唤醒，用于在多线程下同步数据
    std::condition_variable _cond;

    // 管理智能指针的声明周期
    std::vector<std::shared_ptr<std::thread>> _works;
};

int main(int argc, char **argv)
{
    {
        // 鸡蛋工厂
        factory_eggs factory;

        // 打印发货后鸡蛋信息
        factory.register_transport([](info_eggs ct) {
            std::string str = "tag: {} size: {} data: {}\n{}";
            std::string str_id;
            for (int i = 0; i < ct.id.size(); i++)
            {
                str_id += "#" + std::to_string(ct.id[i]) + " ";
            }
            str = replace_fix(str)(ct.tag, ct.size, ct.date, str_id);
            // MCLOG($(str));
        });

        // 3秒后自动关闭
        MCLOG("等待工厂关闭")
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    MCLOG("退出程序")
    return 0;
}

/*
进入发货流程 [/home/red/open/github/mcpp/example/20/main.cpp:112]
进入发货流程 [/home/red/open/github/mcpp/example/20/main.cpp:112]
进入发货流程 [/home/red/open/github/mcpp/example/20/main.cpp:112]
进入采集流程 [/home/red/open/github/mcpp/example/20/main.cpp:169]
等待工厂关闭 [/home/red/open/github/mcpp/example/20/main.cpp:231]
退出采集流程 [_que.size(): 退出鸡蛋工厂 [_que.size(): 5006]  [/home/red/open/github/mcpp/example/20/main.cpp:183]5006
]  [/home/red/open/github/mcpp/example/20/main.cpp:99]
退出发货流程 [_que.size(): 0]  [/home/red/open/github/mcpp/example/20/main.cpp:163]
退出发货流程 [_que.size(): 0]  [/home/red/open/github/mcpp/example/20/main.cpp:163]
退出发货流程 [_que.size(): 0]  [/home/red/open/github/mcpp/example/20/main.cpp:163]
线程退出 [/home/red/open/github/mcpp/example/20/main.cpp:76]
线程退出 [/home/red/open/github/mcpp/example/20/main.cpp:76]
线程退出 [/home/red/open/github/mcpp/example/20/main.cpp:76]
线程退出 [/home/red/open/github/mcpp/example/20/main.cpp:76]
退出程序 [/home/red/open/github/mcpp/example/20/main.cpp:235]
*/
