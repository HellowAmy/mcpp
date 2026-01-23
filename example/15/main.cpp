#include <iostream>
#include <functional>
#include <fstream>

#include "mclog.h"

// 写入回调
struct write_file
{
    // 普通函数
    write_file(const std::string &file, void (*fn_write)(std::ofstream &))
    {
        std::ofstream fs(file);
        if (fs.is_open())
        {
            fn_write(fs);
            fs.close();
        }
    }
};

// 读取回调
struct read_file
{
    // function 类型
    read_file(const std::string &file, std::function<void(const std::string &content)> fn_read = nullptr)
    {
        std::string ret;
        std::ifstream fs(file);
        if (fs.is_open())
        {
            std::string buff;
            while (std::getline(fs, buff))
            {
                ret += buff + "\n";
            }
            if (fn_read)
            {
                fn_read(ret);
            }
            fs.close();
        }
    }
};

// 回调的处理函数
void fun_write(std::ofstream &fs)
{
    fs << "食材：猪肉，牛肉，辣椒，白菜，豆腐" << std::endl;
    fs << "配料：酱油，沙姜，葱花，大蒜" << std::endl;
    fs << "主食：大米，面条，馒头" << std::endl;
    MCLOG("写入函数触发");
}

// 回调的处理函数
void fun_read(const std::string &content)
{
    MCLOG("普通函数读取\n" $(content))
}

int main(int argc, char **argv)
{
    std::string filename = "food_list.txt";

    // 回调写入-使用普通函数处理
    write_file(filename, fun_write);

    // 回调读取-使用普通函数处理
    read_file(filename, fun_read);

    // 回调读取-使用匿名函数处理（lambda）
    read_file(filename, [=](const std::string &content) { 
        MCLOG("匿名函数读取\n" $(content)) 
        MCLOG("文件名为: " $(filename)); 
    });

    // lambda 生命周期
    MCLOG("生命周期");
    std::function<void()> fn_copy = nullptr;
    std::function<void()> fn_ref = nullptr;
    {
        int value = 100;
        int *pvalue = &value;
        fn_copy = [=]()
        {
            MCLOG($(value));
        };
        fn_ref = [&]()
        {
            MCLOG($(value));
        };

        MCLOG("销毁前执行");
        fn_copy();
        fn_ref();

        // 模拟为销毁后的数据改变
        // value 被销毁，被销毁的值是未知的，未定义行为
        *pvalue = 200;
    }
    MCLOG("销毁后执行");
    fn_copy();
    fn_ref();

    return 0;
}