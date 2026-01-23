#include <iostream>
#include <memory>

#include "mclog.h"
#include "byte_tmp.h"

// 使用 str 的地址，推荐
void print(const std::string &str)
{
    std::cout << "打印内容: " << str << std::endl;
}

// 复制了 str 会变慢，不推荐
void print_copy(std::string str)
{
    std::cout << "打印内容: " << str << std::endl;
}

// 获取返回值，推荐
int add(int a, int b)
{
    return a + b;
}

// 引用，不推荐
void add_ref(int &a, int &b)
{
    a += b;
}

// 指针，不推荐，这是一种将参数当作返回值的做法
void add_ptr(int *a, int *b)
{
    *a += *b;
}

int main(int argc, char **argv)
{
    MCLOG("计算方式")
    {
        // 传参数，实际为复制
        int a = 10;
        int b = 20;
        int c = add(a, b);
        MCLOG($(a) $(b) $(c));
    }
    {
        // 传引用，实际为地址
        int a = 10;
        int b = 20;
        add_ref(a, b);
        MCLOG($(a) $(b));
    }
    {
        // 传指针，实际为地址
        int a = 10;
        int b = 20;
        add_ptr(&a, &b);
        MCLOG($(a) $(b));
    }

    MCLOG("\n参数传递大小")
    {
        std::string str;
        int a = 10;
        int *ptr_a = &a;
        MCLOG($(sizeof(str)) $(sizeof(a)) $(sizeof(ptr_a)));

        print("hello");
        print_copy("hello");
    }

    MCLOG("\n指针指向")
    {
        int a = 10;
        int b = 20;
        int ref_a = a;
        int *ptr = nullptr;

        MCLOG($(a) $(b) $(ref_a) $(ptr));

        // 获取到 a 的地址
        ptr = &a;

        // 从 ptr 提取数据赋值给 c 变量后使用
        // 等效 int c = a;
        int c = *ptr;
        MCLOG($(a) $(b) $(ref_a) $(ptr) $(*ptr) $(c));

        // 改为获取 b 赋值给 c 变量
        // 等效 c = b;
        ptr = &b;
        c = *ptr;
        MCLOG($(a) $(b) $(ref_a) $(ptr) $(*ptr) $(c));

        // 指向 a 并从 c 获取值
        // 等效 a = c;
        ptr = &a;
        *ptr = c;
        MCLOG($(a) $(b) $(ref_a) $(ptr) $(*ptr) $(c));
    }

    MCLOG("\n指针寻指")
    {
        // ptr 地址推进步长为 4 ，buf 的总长度为 40
        int *ptr = nullptr;
        int buf[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

        // 获取指定位置
        ptr = &buf[2];
        MCLOG($(*ptr));

        // 向前推进 2 个位置，因为数组的数据是连续的，向前推进还在数据地址内
        // 推进的步长为 4 * 2
        ptr += 2;
        MCLOG($(*ptr));

        // 转为大块内存
        long long *ptrll = reinterpret_cast<long long *>(ptr);
        std::string str_bit = print_byte<long long>(*ptrll);
        MCLOG($(*ptrll));
        MCLOG($(str_bit));

        /*
            分析 ptrll 指向的二进制 str_bit
            0000000000000000000000000000011000000000000000000000000000000101
            00000000 00000000 00000000 00000110 00000000 00000000 00000000 00000101
                                            6                                   5
            ptrll 内存顺序是从高往底，指向的是 5 6 两个数字地址，ptrll 等于  buf[4] buf[5] 两块内存的合并
        */

        // 大块内存推进 1 个位置，之后转为小内存
        // 推进的步长为 8 * 1
        // ptrll 推进之后指向 buf[6] buf[7] ，转为 ptr 之后指向 buf[6] 从低到高转为小内存
        // ptrll 推进 1 等于 ptr 推进 2，因为 ptrll 的地址长度是 ptr 的一倍
        ptrll++;
        ptr = reinterpret_cast<int *>(ptrll);
        MCLOG($(*ptr));

        // 修改指向地址的数据，改变二进制数据为 15 这个数字
        *ptrll = 0b0000000000000000000000000000111100000000000000000000000000001111;
        for (int i = 0; i < (sizeof(buf) / sizeof(int)); i++)
        {
            std::cout << buf[i] << " ";
        }
        std::cout << std::endl;
    }

    MCLOG("\n分配空间")
    {
        // 分配空间，不推荐，需要手动释放，忘记就会内存泄露
        int *ptr_a = new int(10);
        int *ptr_b = new int(20);

        // 智能指针，推荐，不用手动释放
        std::shared_ptr<int> sp_a = std::make_shared<int>(10);
        std::shared_ptr<int> sp_b = std::make_shared<int>(20);

        MCLOG($(*ptr_a) $(*ptr_b) $(ptr_a) $(ptr_b));
        MCLOG($(*sp_a) $(*sp_b) $(sp_a) $(sp_b));

        // 销毁
        delete ptr_a;
        delete ptr_b;
    }

    MCLOG("\n垃圾代码")
    {
        int buf[3][3][3]{
            {{1, 2, 3}, {10, 20, 30}, {100, 200, 300}},
            {{4, 5, 6}, {40, 50, 60}, {400, 500, 600}},
            {{7, 8, 9}, {70, 80, 90}, {700, 800, 900}},
        };

        int *pl1 = &buf[0][0][0];
        int (*pl2)[3] = &buf[0][0];
        int (*pl3)[3][3] = &buf[0];

        // 垃圾代码
        (pl1 = *((pl2 = *(++pl3)) += 2))++;
        MCLOG($(*pl1) $(*pl2[0]) $(*pl3[0][0]));

        pl1 = &buf[0][0][0];
        pl2 = &buf[0][0];
        pl3 = &buf[0];

        // 等效代码
        pl3++;
        pl2 = pl3[0];
        pl2 += 2;
        pl1 = pl2[0];
        pl1++;
        MCLOG($(*pl1) $(*pl2[0]) $(*pl3[0][0]));

        // 等效代码，直接定位到 [2-1][3-1][1]
        pl1 = &buf[0][0][0];
        pl1 += 3 * (2 + 3) + 1;
        MCLOG($(*pl1));

        /*
            代码分析
                数据范围
                {{1, 2, 3}, {10, 20, 30}, {100, 200, 300}},
                  ^
                 pl1
                {{4, 5, 6}, {40, 50, 60}, {400, 500, 600}},
                {{7, 8, 9}, {70, 80, 90}, {700, 800, 900}},

            执行: ++pl3
            等价: pl3 += 1
                数据范围
                {{4, 5, 6}, {40, 50, 60}, {400, 500, 600}},
                  ^
                 pl1
            执行: (pl2 = *(++pl3)) += 2
            等价: pl2 += 2
                数据范围
                {{4, 5, 6}, {40, 50, 60}, {400, 500, 600}},
                                           ^
                                          pl1
            执行: (pl1 = *((pl2 = *(++pl3)) += 2))++
            等价: pl1 += 1
                数据范围
                {400, 500, 600}
                      ^
                     pl1
        */

        // 遍历
        MCLOG("遍历结果");
        for (int a = 0; a < 3; a++)
        {
            for (int b = 0; b < 3; b++)
            {
                for (int c = 0; c < 3; c++)
                {
                    std::cout << buf[a][b][c] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    return 0;
}