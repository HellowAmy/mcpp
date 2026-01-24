#include <iostream>
#include <cstring>
#include <vector>
#include <vector>
#include "mclog.h"

// 存放字符类型的结构体，用于判断和格式化字符串
struct enum_type
{
    static const char digit = 'i';
    static const char chars = 's';
    static const char in = '[';
    static const char out = ']';
    static const char ready = ':';
};

// 用于保存二进制数据，可以是任何类型的类型
struct data_save
{
    int size = 0;
    void *data = nullptr;
};

// 创建一个结构体用于保存数据类型
// 存在 next 变量的同类型结构，说明 type_input 是链表结构，单向链表
// type 保存的类型
// data 具体的数据
// next 连续存储的下一个位置，指向同类型
struct type_input
{
    char type = '0';
    data_save data;
    type_input *next = nullptr;
};

// 处理数据输入格式并保存数据的类
class data_storage
{
public:
    // 初始化总数量为可以存储一个值
    data_storage()
    {
        resize_arr(1);
    }

    // 释放所以手动分配的空间，打印是否存在内存泄露
    ~data_storage()
    {
        delete_arr();
        MCLOG("未释放内存次数 " $(_size_allo));
    }

    // 解析字符串获取到 type_input 的结构列表，存储到 arr 数组中
    void parse_input(const std::string &str)
    {
        type_input *input = push_input_list(str);
        show_input(input);
        push_arr(input);
    }

    // show_input 的重载函数
    void show_input()
    {
        for (int i = 0; i < _size_arr; i++)
        {
            show_input((type_input *)_arr[i]);
        }
    }

    // show_input 的重载函数
    void show_input(int index)
    {
        if (index < _size_arr)
        {
            show_input((type_input *)_arr[index]);
        }
    }

private:
    // 从 type_input 链表获取一次存入的多个数据，输出成一行显示
    void show_input(type_input *input)
    {
        std::cout << "储存内容: ";
        type_input *go = input;
        
        // 从链表第一个表头位置开始，逐步遍历所有内容
        // 如果为空则表示链表结束了，不执行并退出
        while (go != nullptr)
        {
            // 根据具体类型，格式化成数字或文本
            if (go->type == enum_type::digit)
            {
                int value = *(int *)go->data.data;
                std::cout << "[数字: " << value << "]  ";
            }
            else if (go->type == enum_type::chars)
            {
                std::string str((char *)go->data.data, go->data.size);
                std::cout << "[文本: " << str << "]  ";
            }

            // 获取指向下一个 type_input 数据的指针赋值给自己，遍历行为
            go = go->next;
        }
        std::cout << std::endl;
    }

    // 将链表头放入 arr 数组
    void push_arr(type_input *input)
    {
        _arr[_size_arr] = input;
        _size_arr++;

        // arr 数组空间不足时自动扩容，类似 vector 行为
        if (_size_arr == _cap_arr)
        {
            resize_arr(_cap_arr * 2);
        }
    }

    // 从字符串中解析出 type_input 链表结构，可能存在有多个 type_input 数据
    // 函数会返回 type_input 链表的表头，可以根据表头遍历所有的数据
    type_input *push_input_list(const std::string &str)
    {
        // 定义 head 链表头和 next 跟进节点指针
        type_input *head = nullptr;
        type_input *next = nullptr;

        // 格式化判断准备
        std::string data;
        bool in = false;
        bool ready = false;
        char type = '0';
        for (int i = 0; i < str.size(); i++)
        {
            // 单次遍历所有传入的字符串
            char go = str[i];
            if (go == enum_type::in)
            {
                // 进入分割点，每一段数据都需要满足这个分隔点的格式才可以存储
                in = true;
            }
            else if (in && go == enum_type::out)
            {
                // 退出分割点，已经完成第一段的数据截取，开始从截取的数据中分析内容
                // 分配一个 type_input 的数据结构用于存储截取内容
                type_input *input = (type_input *)new_mem(sizeof(type_input));
                if (head == nullptr)
                {
                    head = input;
                }
                if (next)
                {
                    next->next = input;
                }

                // 链表操作，用于指向下一个 type_input 数据，下一次进入时就可以自己从尾部追加
                // head 表头指针是不变的，next 下一个跟随指针总是会移动到链表的尾部
                next = input;
                input->next = nullptr;
                input->type = type;

                if (type == enum_type::chars)
                {
                    // 文本格式，分配内容并复制到 type_input 的空间
                    int len = data.size();
                    char *p = (char *)new_mem(len);
                    std::memcpy(p, data.c_str(), data.size());
                    input->data.data = p;
                    input->data.size = len;
                }
                else if (type == enum_type::digit)
                {
                    // 数字格式，分配内容并复制到 type_input 的空间
                    int num = std::stoi(data);
                    int len = sizeof(int);
                    int *p = (int *)new_mem(len);
                    *p = num;
                    input->data.data = p;
                    input->data.size = len;
                }

                // 第一次截取完成，缓存状态归零，准备下一次截取
                in = false;
                ready = false;
                data.clear();
            }
            else if (in && go == enum_type::ready)
            {
                // 准备截取数据
                ready = true;
            }
            else if (in && ready == false)
            {
                // 获取数字或文本类型
                type = go;
            }
            else if (in && ready)
            {
                // 正在截取数据，缓存数据中
                data.push_back(go);
            }
        }

        return head;
    }

    // 释放 type_input 链表结构，从头开始逐个释放分配的数据
    void delete_input_list(type_input *input)
    {
        type_input *go = input;
        while (go != nullptr)
        {
            type_input *tm = go;
            go = go->next;
            delete_mem(tm->data.data);
            delete_mem(tm);
        }
    }

    // 给 arr 数组分配空间，会分配一块新空间，将旧空间赋值到新空间之后，释放旧空间
    void resize_arr(size_t size)
    {
        void **p = (void **)new_mem(size * sizeof(void *));
        for (int i = 0; i < _size_arr; i++)
        {
            p[i] = _arr[i];
        }
        delete_mem(_arr);

        _arr = p;
        _cap_arr = size;
        MCLOG("重新分配空间" $(_cap_arr) $(_size_arr))
    }

    // 释放所有的 arr 数组内容，包括存储在数组上的 type_input 链表结构数据
    void delete_arr()
    {
        for (int i = 0; i < _size_arr; i++)
        {
            delete_input_list((type_input *)_arr[i]);
        }

        delete_mem(_arr);
        _size_arr = 0;
        _cap_arr = 0;
        _arr = nullptr;
    }

    // 统一的释放函数，记录次数
    void delete_mem(void *p)
    {
        if (p != nullptr)
        {
            _size_allo--;
            delete[] (char *)p;
        }
    }

    // 统一的分配函数，记录次数
    void *new_mem(size_t len)
    {
        void *p = new char[len];
        _size_allo++;
        return p;
    }

private:
    // 分配记录，用于查看是否内存泄露
    int _size_allo = 0;

    // arr 数组当前储存的个数
    int _size_arr = 0;

    // arr 数组当前分配的总空间
    int _cap_arr = 0;

    // _arr 是数组，可以存储 void* 指针类型的数据，用于存储 type_input 数据列表的表头
    // 拥有表头之后，可以遍历所有的内容，可以掌握所有的 type_input 数据
    void **_arr = nullptr;
};

int main(int argc, char **argv)
{
    MCLOG("测试环节");
    {
        data_storage storage;
        storage.parse_input("[i:123][s:qwe][i:456][i:789][s:hello][s:world]");
        storage.parse_input("[i:100]");
        storage.parse_input("[i:200][i:300]");
        storage.parse_input("[s:hello]");
        storage.show_input();
    }

    MCLOG("\n用户操作");
    {
        bool run = true;
        int id = 0;
        std::string buff;
        data_storage storage;
        while (run)
        {
            std::string str = "\n欢迎来到数据存储机\n"
                              "1.存储\n"
                              "2.查看\n";
            std::cout << str << std::endl;
            std::cin >> buff;

            if (buff == "quit")
            {
                run = false;
            }
            else if (buff == "1")
            {
                std::cout << "存储格式 [i:XXX][s:XXX]" << std::endl;
                std::cin >> buff;
                storage.parse_input(buff);
                std::cout << "存储成功，请记住你的ID，下次查看时使用，ID: " << id << std::endl;
            }
            else if (buff == "2")
            {
                std::cout << "请输出要查看的ID" << std::endl;
                std::cin >> buff;
                int index = std::stoi(buff);
                storage.show_input(index);
            }
            id++;
        }
        MCLOG("退出程序");
    }
    return 0;
}
