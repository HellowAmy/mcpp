#include "rand_int.h"

// 实现 rand_int 构造函数
rand_int::rand_int(int min, int max) : _unifi(min, max)
{
    _gen.seed((std::random_device())());
}

// 实现 value 函数，生成随意数
int rand_int::value()
{
    return _unifi(_gen);
}