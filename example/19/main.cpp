#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <memory>
#include <chrono>

#include "mclog.h"

// 一百万
int count_100w = 100 * 10000;

// 无锁累加变量
int num = 0;

// 原子累加变量
std::atomic<int> atm_num(0);

// 使用 mutex 上锁的普通变量
int num_lock = 0;
std::mutex mut;

// 测试不同类型运算的多线程函数
void work_add_th()
{
    for (int i = 0; i < count_100w; i++)
    {
        num++;
    }
    MCLOG("work_add 结束");
}
void work_sub_th()
{
    for (int i = 0; i < count_100w; i++)
    {
        num--;
    }
    MCLOG("work_sub 结束");
}
void work_add_atomic_th()
{
    for (int i = 0; i < count_100w; i++)
    {
        atm_num++;
    }
    MCLOG("work_add_atomic 结束");
}
void work_sub_atomic_th()
{
    for (int i = 0; i < count_100w; i++)
    {
        atm_num--;
    }
    MCLOG("work_sub_atomic 结束");
}

void work_add_lock_th()
{
    for (int i = 0; i < count_100w; i++)
    {
        // lock 会在下一行代码上锁，离开作用域自己解锁
        // 在这个 for 循环中，每一个循环都是单独的作用域，会自动上锁和解锁一次
        std::lock_guard<std::mutex> lock(mut);
        num_lock++;
    }
    MCLOG("work_add_lock 结束");
}
void work_sub_lock_th()
{
    for (int i = 0; i < count_100w; i++)
    {
        std::lock_guard<std::mutex> lock(mut);
        num_lock--;
    }
    MCLOG("work_sub_lock 结束");
}

int main(int argc, char **argv)
{
    // 线程的意义
    MCLOG("单线程与多线程速度对比");
    {
        using namespace std::chrono;

        // 1 亿
        int count_100m = 10 * 10000 * 10000;

        // 单线程速度
        {
            auto btime = steady_clock::now();
            long long sum = 0;
            for (int i = 0; i < count_100m; i++)
            {
                sum += i;
            }
            auto etime = duration_cast<milliseconds>(steady_clock::now() - btime).count();
            MCLOG("单线程毫秒 " $(etime) $(sum));
        }

        // 多线程速度
        {
            auto btime = steady_clock::now();
            int half = count_100m / 2;
            long long sum_fisrt = 0;
            long long sum_second = 0;

            // 多线程计算
            std::thread th1([&]() {
                long long sum_fisrt_1 = 0;
                for(int i=0;i<half;i++)
                {   
                    sum_fisrt_1+=i;
                }
                sum_fisrt = sum_fisrt_1; 
            });
            std::thread th2([&]() {
                long long sum_fisrt_2 = 0;
                for(int i=half;i<count_100m;i++)
                {   
                    sum_fisrt_2+=i;
                }
                sum_second = sum_fisrt_2; 
            });

            // 等待同步
            th1.join();
            th2.join();
            long long sum = sum_fisrt + sum_second;

            auto etime = duration_cast<milliseconds>(steady_clock::now() - btime).count();
            MCLOG("多线程毫秒 " $(etime) $(sum));
        }
    }

    // 线程的未知数
    MCLOG("\n多线程数据竞争问题");
    {
        MCLOG("无锁多线程启动");
        std::thread th_add(work_add_th);
        std::thread th_sub(work_sub_th);

        MCLOG("等待线程");
        th_add.join();
        th_sub.join();
        MCLOG("执行完毕 " $(num));
    }
    {
        MCLOG("\n原子变量多线程启动");
        std::thread th_add(work_add_atomic_th);
        std::thread th_sub(work_sub_atomic_th);

        MCLOG("等待线程");
        th_add.join();
        th_sub.join();
        MCLOG("执行完毕 " $(atm_num));
    }
    {
        MCLOG("\n上锁多线程启动");
        std::thread th_add(work_add_lock_th);
        std::thread th_sub(work_sub_lock_th);

        MCLOG("等待线程");
        th_add.join();
        th_sub.join();
        MCLOG("执行完毕 " $(num_lock));
    }
    return 0;
}
