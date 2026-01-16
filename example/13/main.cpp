#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <forward_list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "mclog.h"

// 普遍版本转字符串
template <typename T>
std::string to_str(const T &val)
{
    return std::to_string(val);
}

// 重载版本，不处理 std::string 类型
std::string to_str(const std::string &str)
{
    return str;
}

// 重载版本，处理 pair 键值对版本，引用普遍版本转字符串
template <typename T1, typename T2>
std::string to_str(const std::pair<T1, T2> &val)
{
    return "[" + to_str(val.first) + ": " + to_str(val.second) + "]";
}

// 格式化打印
template <typename T>
void print_cont(const T &con, std::string label = "")
{
    // 使用快捷遍历方式遍历
    // 该方法需要存在前进迭代器，依旧返回的对象可直接使用
    // 使用 to_str 将 ele 的数据转为字符串
    std::cout << "| label: " << label << std::endl;
    for (auto &ele : con)
    {
        std::cout << "| " << to_str(ele) << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    // 打印方式展示
    {
        MCLOG("打印方式展示，用于做后续的对比输出")
        std::vector<int> vec;

        // 尾部追加 1~5 的整数
        for (int i = 0; i < 5; i++)
        {
            vec.push_back(i + 100);
        }

        // 使用下标遍历
        MCLOG("常规打印")
        for (size_t i = 0; i < vec.size(); i++)
        {
            std::cout << vec[i] << std::endl;
        }

        // 使用快捷遍历，一次性遍历所有元素
        // 每一次遍历的元素都存在 ele 元素中
        MCLOG("快捷遍历")
        for (auto &ele : vec)
        {
            std::cout << ele << std::endl;
        }

        // 使用模板函数 print_cont 进行遍历，统一格式化打印结果
        MCLOG("print_cont 函数打印")
        print_cont(vec);
    }

    // array
    {
        // 数组，需要指定分配连续的空间
        std::array<int, 5> arr;
        MCLOG("使用 array 容器，初始化数量为 5")
        MCLOG("初始化后容器大小 " $(arr.size()));

        // 赋值需要根据 array 容器大小决定
        for (int i = 0; i < arr.size(); i++)
        {
            arr[i] = i + 100;
        }

        MCLOG("赋值后容器大小 " $(arr.size()));
        MCLOG("通过下标取值 " $(arr[2]));
        print_cont(arr, "array 容器");
    }

    // vector
    {
        // 动态数组，可以自动扩展容量，数据是连续存放的
        std::vector<int> vec;
        MCLOG("使用 vector 容器")
        MCLOG("初始化后容器大小 " $(vec.size()));

        // 赋值 5 次
        for (int i = 0; i < 5; i++)
        {
            vec.push_back(i + 100);
        }

        MCLOG("赋值后容器大小 " $(vec.size()));
        MCLOG("通过下标取值 " $(vec[2]));

        vec.erase(vec.begin() + 1);
        MCLOG("删除一个值后大小 " $(vec.size()));

        MCLOG("容器数量大小 " $(vec.size()));
        MCLOG("实际内存分配 " $(vec.capacity()));

        print_cont(vec, "vector 容器");
    }

    // list
    {
        // 双向链表，数据存储在下一个指针指向的单独地址，数据不连续
        std::list<int> ls;
        MCLOG("使用 list 容器")
        MCLOG("初始化后容器大小 " $(ls.size()));
        for (int i = 0; i < 5; i++)
        {
            ls.push_back(i + 100);
        }

        // 无法通过下标查询，需要通过迭代器查询
        // 前进3步
        auto go = ls.begin();
        for (int i = 0; i < 3; i++)
        {
            go++;
        }
        MCLOG("前进3步 " $(*go));

        // 后退1步
        go--;
        MCLOG("后退1步 " $(*go));

        MCLOG("赋值后容器大小 " $(ls.size()));
        print_cont(ls, "list 容器");
    }

    // forward_list
    {
        // 单向链表，比 list 的双向链表要快
        std::forward_list<int> fls;
        MCLOG("使用 forward_list 容器")
        for (int i = 0; i < 5; i++)
        {
            fls.push_front(i + 100);
        }

        // forward_list 是单向列表，只能向前访问下一位，无法后退
        // 前进3步
        auto go = fls.begin();
        for (int i = 0; i < 3; i++)
        {
            go++;
        }
        MCLOG("前进3步 " $(*go));

        // 遍历 forward_list ，forward_list 没有 size 函数
        print_cont(fls, "forward_list 容器");
    }

    // deque
    {
        // 用法与 vector 类似，可以访问下标
        // deque 储存的数据不是连续的，而是分段存储
        // 但是 deque 可以实现逻辑上的数据连续，从而可以访问下标
        // deque 可以从两侧高效的插入和删除 vector 只能在尾部高效的插入和删除
        std::deque<int> deq;
        MCLOG("使用 deque 容器")
        MCLOG("初始化后容器大小 " $(deq.size()));
        for (int i = 0; i < 5; i++)
        {
            deq.push_back(i + 100);
        }

        MCLOG("通过下标取值 " $(deq[2]));

        deq.push_back(10);
        deq.push_front(20);

        MCLOG("赋值后容器大小 " $(deq.size()));
        print_cont(deq, "deque 容器");
    }

    // queue
    {
        // queue 队列，继承自 deque 数据结构
        // 实际上 queue 是一种封装，而不是新的数据结构类型
        // queue 约束数据只能从尾部进，头部出，不能访问中间的数据，有先进先出的特点
        // queue 容器是封闭的，只能看第一个和最后一个数据
        std::queue<int> que;
        MCLOG("使用 queue 容器")
        MCLOG("初始化后容器大小 " $(que.size()));
        for (int i = 0; i < 5; i++)
        {
            que.push(i + 100);
        }

        MCLOG("赋值后容器大小 " $(que.size()));

        while (que.size() > 0)
        {
            int val = que.front();
            que.pop();
            std::cout << "| " << val << std::endl;
        }
        std::cout << std::endl;
    }

    // stack
    {
        // stack 栈结构，继承自 deque 数据结构
        // stack 和 queue 都是 deque 的封装，且特点与 queue 几乎相同，也是封闭的
        // 不同的是 stack 是先进后出，queue 是先进先出
        // stack 的底部是封上的
        std::stack<int> stk;
        MCLOG("使用 stack 容器")
        MCLOG("初始化后容器大小 " $(stk.size()));
        for (int i = 0; i < 5; i++)
        {
            stk.push(i + 100);
        }

        MCLOG("赋值后容器大小 " $(stk.size()));

        while (stk.size() > 0)
        {
            int val = stk.top();
            stk.pop();
            std::cout << "| " << val << std::endl;
        }
        std::cout << std::endl;
    }

    // map
    {
        // 红黑树树结构（red-black-tree）数据总是可以通过key值快速找到
        std::map<int, std::string> mp;
        MCLOG("使用 map 容器")
        MCLOG("初始化后容器大小 " $(mp.size()));
        for (int i = 0; i < 5; i++)
        {
            mp.emplace(i + 100, std::string("value" + std::to_string(i)));
        }

        // 下标法访问，添加一个值
        mp[10] = "value10";

        // 添加一个值，键值对的方式，很麻烦不推荐
        std::pair<int, std::string> pr(20, "value20");
        mp.insert(pr);

        MCLOG("获取一个值-存在 " << $(mp[10]))

        // 使用下标法访问时，不存在时会自动添加一个空的新值
        MCLOG("获取一个值-不存在 " << $(mp[30]))

        // 使用 at 函数获取，不存在会抛异常
        try
        {
            mp.at(40);
        }
        catch (...)
        {
            MCLOG("出现异常，不存在 40 这个 key")
        }

        // 添加已存在的值为替换
        mp[10] = "new value 10";
        MCLOG("替换值 " << $(mp[10]))

        MCLOG("赋值后容器大小 " $(mp.size()));
        print_cont(mp, "map 容器");
    }

    // set
    {
        // 与 map 相似的结构，但是没有 value 只有 key
        std::set<int> st;
        MCLOG("使用 set 容器")
        MCLOG("初始化后容器大小 " $(st.size()));
        for (int i = 0; i < 5; i++)
        {
            st.emplace(i + 100);
        }

        // 重复插入会失败，常见用法是将 vector 的内容插入 set 来排除重复值
        st.insert(10);
        st.insert(10);
        st.insert(10);

        MCLOG("赋值后容器大小 " $(st.size()));
        print_cont(st, "set 容器");
    }

    // unordered_multimap
    {
        // hash 结构，总是能更快的查找数据
        std::unordered_multimap<int, std::string> mulmp;
        MCLOG("使用 unordered_multimap 容器")
        MCLOG("初始化后容器大小 " $(mulmp.size()));
        for (int i = 0; i < 5; i++)
        {
            mulmp.emplace(i + 100, std::string("value" + std::to_string(i)));
        }

        // unordered_multimap 是可以存储相同 key 的 map 版本，底层使用 hash 数据结构
        // map 底层使用红黑树结构
        // 插入相同key的内容
        mulmp.emplace(10, "value10");
        mulmp.emplace(10, "new value 10");
        mulmp.emplace(10, "not 10");

        // 查看 key 为 10 的值，会返回多个结果
        // 返回值是开始和结束的pair迭代器
        auto pr = mulmp.equal_range(10);
        for (auto it = pr.first; it != pr.second; it++)
        {
            std::string value = (*it).second;
            MCLOG("查看 key 10 的值 " $(value));
        }

        MCLOG("赋值后容器大小 " $(mulmp.size()));
        print_cont(mulmp, "unordered_multimap 容器");
    }

    // 最好总结 map set 的多种容器变体
    // unordered 代表 hash 结构， mul 代表可以可重复存储数据
    // 没有前缀代表不可重复的红黑树类型
    {
        // 红黑树，不可重复，带键值对 key - value
        std::map<int, std::string> mp;

        // 红黑树，可重复
        std::multimap<int, std::string> mmp;

        // 哈希表（hash），不可重复
        std::unordered_map<int, std::string> ump;

        // 哈希表，可重复
        std::unordered_multimap<int, std::string> ummp;

        // 红黑树，不可重复，只有值 value
        std::set<int> st;

        // 红黑树，可重复
        std::multiset<int> mst;

        // 哈希表，不可重复
        std::unordered_set<int> ust;

        // 哈希表，可重复
        std::unordered_multiset<int> umst;
    }

    return 0;
}