#ifndef RAND_INT_H
#define RAND_INT_H

#include <random>

// 定义整数随机类
class rand_int
{
public:
    // 声明构造函数，未实现
    rand_int(int min, int max);

    // 声明获取随机数，未实现
    int value();

private:
    // 分配器
    std::uniform_int_distribution<int> _unifi;

    // 算法
    std::mt19937_64 _gen;
};
#endif // RAND_INT_H
