#include <iostream>

#include "mclog.h"
#include "replace_args.h"

// 存储坐标内容
class coord
{
public:
    // 无参数构造函数
    coord() {}

    // 带参数构造函数
    coord(int x, int y, int z)
        : _x(x), _y(y), _z(z) {}

    // 重载大于号对比大小
    bool operator>(const coord &other)
    {
        return cmp_greater(other);
    }

    // 重载对象本身
    operator std::string()
    {
        return to_string();
    }

    // 重载左移为按序赋值
    coord &operator<<(int num)
    {
        if (_x == 0)
        {
            _x = num;
        }
        else if (_y == 0)
        {
            _y = num;
        }
        else if (_z == 0)
        {
            _z = num;
        }
        return *this;
    }

    // 等效 bool operator>(const coord &other)
    bool cmp_greater(const coord &other)
    {
        return sum() > other.sum();
    }

    // 等效 operator std::string()
    std::string to_string()
    {
        std::string str = "[x:{0} y:{1} z:{2}]";
        str = replace_args(str)(
            "{0}", std::to_string(_x),
            "{1}", std::to_string(_y),
            "{2}", std::to_string(_z));
        return str;
    }

    // 计算总数，存在 const 声明可不变
    // 可让 const coord &other 不可变对象调用，否则无法调用
    int sum() const
    {
        return _x + _y + _z;
    }

private:
    int _x = 0;
    int _y = 0;
    int _z = 0;
};

int main(int argc, char **argv)
{
    {
        MCLOG("大于号重载 bool operator>(const coord &other)");
        coord obj1(10, 20, 30);
        coord obj2(100, 200, 300);

        // obj1 > obj2 可用，因为重载了
        // obj1 < obj2 不可用，还没有重载
        if (obj1 > obj2)
        {
            MCLOG("obj1 > obj2");
        }
        else
        {
            MCLOG("obj2 > obj1");
        }
    }
    {
        MCLOG("\n左移重载 coord &operator<<(int num)");
        coord obj1;
        coord obj2;
        obj2 << 10 << 20 << 30;

        MCLOG($(obj1.sum()) $(obj2.sum()));
    }
    {
        MCLOG("\n对象重载 operator std::string()");
        coord obj1(10, 20, 30);
        coord obj2(100, 200, 300);
        std::string str1 = obj1;
        std::string str2 = obj2;

        // 调用拷贝构造函数
        coord obj3 = obj1;

        // 调用 std::string to_string() 重载
        std::string str3 = obj3;

        MCLOG($(str1) $(str2) $(str3));
    }

    return 0;
}