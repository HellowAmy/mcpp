#include <iostream>
#include <chrono>
#include <random>
#include <thread>

#include "mclog.h"
#include "rand_int.h"

int main(int argc, char **argv)
{
    MCLOG("随机数部分")

    // 准备随机算法
    // 梅森旋转算法，推荐使用
    std::mt19937_64 gen;

    // 其他可选算法
    // std::mt19937 gen;
    // std::minstd_rand gen;
    // std::ranlux48 gen;

    // 准备伪随机数种子
    // 设备提供的随机种子，推荐使用
    std::random_device dev;
    gen.seed(dev());

    // 固定时间种子，每次生成的数据都一样
    // gen.seed(44);

    // 随机时间种子
    // gen.seed(std::chrono::system_clock::now().time_since_epoch().count());

    // 随机序列分配器
    // 从随机数中分配数据类型 int
    // 会生成包括 -3 和 3 的数字
    std::uniform_int_distribution<int> unifi(-3, 3);
    for (int i = 0; i < 10; i++)
    {
        int num = unifi(gen);
        MCLOG("整数 " << $(num));
    }

    // 从随机数中分配数据类型 double
    std::uniform_real_distribution<double> unifr(0.5, 1.0);
    for (int i = 0; i < 5; i++)
    {
        double num = unifr(gen);
        MCLOG("浮点 " << $(num));
    }

    //  快速使用
    rand_int rand(20, 80);
    for (int i = 0; i < 5; i++)
    {
        int num = rand.value();
        MCLOG("快速使用 " << $(num));
    }

    // 常用的分配器
    // uniform_int_distribution     int     均匀-整数分布
    // uniform_real_distribution    double  均匀-浮点分布
    // bernoulli_distribution       bool    均匀-布尔分布
    // normal_distribution          double  正态-距离中间点扩散概率
    // discrete_distribution        int     离散-列表中下标出现概率
    // poisson_distribution         int     泊松-模拟一小时内发生次数分布
    // gamma_distribution           double  泊松-模拟所有事件发生总长分布

    // 时间部分
    MCLOG("\n时间部分")

    // 开启命名空间
    using namespace std::chrono;

    // 定义线性时间
    steady_clock clock;

    // 睡眠 100 ms 时间
    time_point<steady_clock> tbegin = clock.now();
    std::this_thread::sleep_for(milliseconds(100));
    time_point<steady_clock> tend = clock.now();

    // 打印时间
    auto tspace = tend - tbegin;
    MCLOG("纳秒 " $(tspace.count()));

    // 转为毫秒
    auto ms = duration_cast<milliseconds>(tspace).count();
    MCLOG("毫秒 " $(ms));

    // 常见的时间单位
    // nanoseconds  纳秒
    // microseconds 微妙
    // milliseconds 毫秒
    // seconds      秒
    // minutes      分
    // hours        时

    // 制作一个简单的随机定时任务
    MCLOG("\n定时清理任务")

    // 任务列表
    std::vector<std::string> vec{"洗衣服", "拖地", "去冰箱结块", "清理桌面", "打扫房间", "喝可乐"};

    // 假设每个小时生成一次任务，从早上八点到下午五点
    rand_int rand_task(0, vec.size() - 1);
    auto tbegin_task = clock.now();
    for (int time = 8; time < 17; time++)
    {
        std::string task = vec[rand_task.value()];
        MCLOG("你需要执行的任务 " $(task) $(time));

        // 休眠一个小时
        // std::this_thread::sleep_for(hours(1));
        std::this_thread::sleep_for(milliseconds(100));
    }
    auto tspace_task = duration_cast<milliseconds>(clock.now() - tbegin_task).count();
    MCLOG("清理任务总计时间 " $(tspace_task));

    return 0;
}