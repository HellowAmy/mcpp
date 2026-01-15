#include <iostream>
#include <fstream>

#include "mclog.h"

int main(int argc, char **argv)
{
    // 定义文件名称，没有执行路径，文件会在执行目录下生成
    std::string file_fstream = "file_fstream.txt";

    // std::ios::out 重写文本文件，以前的内容直接被清空
    // 代码被 { } 空作用域包围，用于限制变量名有效范围，变量名称一旦离开作用域就会被销毁
    {
        // 创建文件操作对象
        std::fstream fs(file_fstream, std::ios::out);

        // 判断是否成功打开
        if (fs.is_open())
        {
            fs << "由 fstream 类型打开\n";
            fs << "hello world!";

            // 关闭文件
            fs.close();
        }
    }

    // std::ios::in 读取文本文件，不可以写入，但是可以读取
    {
        MCLOG("ifstream 第一次读取");
        std::fstream fs(file_fstream, std::ios::in);
        if (fs.is_open())
        {
            std::string buff;
            while (std::getline(fs, buff))
            {
                MCLOG($(buff));
            }
            fs.close();
        }
    }

    // 使用 std::ofstream 默认自带 std::ios::out 参数，可以直接写入
    // std::ios::app 追加文本文件，从文件中读取原本内容后添加新内容
    {
        std::ofstream fs(file_fstream, std::ios::app);
        if (fs.is_open())
        {
            fs << "\n追加了一段内容";
            fs.close();
        }
    }

    // 使用 ifstream 默认自带 std::ios::in 参数，可以读取内容
    // 读取查看是否成功追加
    {
        MCLOG("\nifstream 第二次读取");
        std::ifstream fs(file_fstream);
        if (fs.is_open())
        {
            std::string buff;
            while (std::getline(fs, buff))
            {
                MCLOG($(buff));
            }
            fs.close();
        }
    }

    // 声明坐标结构
    struct coord
    {
        int x = 0;
        int y = 0;
        int z = 0;
    };

    // 定义存储二进制数据的文件名称
    std::string file_file_binary = "file_file_binary.txt";

    // std::ios::out | std::ios::binary 重写内容与二进制模式，写入二进制内容到文件
    {
        std::fstream fs(file_file_binary, std::ios::out | std::ios::binary);
        if (fs.is_open())
        {
            // 将这个坐标的数据的二进制数据转移到文件
            coord obj;
            obj.x = 210;
            obj.y = 73;
            obj.z = 16;
            fs.write(reinterpret_cast<const char *>(&obj), sizeof(obj));
            fs.close();
        }
    }

    // std::ios::in | std::ios::binary 读取与二进制模式，从文件读取二进制内容
    {
        MCLOG("\n读取二进制文件数据");
        std::fstream fs(file_file_binary, std::ios::in | std::ios::binary);
        if (fs.is_open())
        {
            // 将二进制内容复制到 x 变量的地址，需要确认二进制的内容是否有效
            std::string buff;
            coord obj;
            fs.read(reinterpret_cast<char *>(&obj), sizeof(obj));
            fs.close();

            MCLOG($(obj.x) $(obj.y) $(obj.z));
        }
    }

    return 0;
}