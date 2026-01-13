#include <iostream>

#include "mclog.h"
#include "format.h"
#include "byte_tmp.h"

// 整数枚举类型-可进行整数转换
// 枚举星期一到星期天的类型
enum en_weekday
{
    e_monday = 1,
    e_tuesday = 2,
    e_wednesday = 3,
    e_thursday = 4,
    e_friday = 5,
    e_saturday = 6,
    e_sunday = 7,
};

// 强制枚举类型-不可进行整数转换
// 枚举出行可选的交通方式
enum class en_way
{
    e_walk,
    e_bicycle,
    e_busy,
    e_car,
};

// 结构体类型
// 声明名为购买检查项的结构体
struct shopping_check
{
    int money = 0;
    int time = 0;
    int distance = 0;
    en_way way = en_way::e_walk;
    std::string describe;
};

// 根据枚举值转化字符串
std::string to_str_enum(en_weekday en)
{
    switch (en)
    {
    case e_monday:
    {
        return "星期一";
    }
    case e_tuesday:
    {
        return "星期二";
    }
    case e_wednesday:
    {
        return "星期三";
    }
    case e_thursday:
    {
        return "星期四";
    }
    case e_friday:
    {
        return "星期五";
    }
    case e_saturday:
    {
        return "星期六";
    }
    case e_sunday:
    {
        return "星期天";
    }
    default:
        break;
    }
    return "NULL";
};

// to_str_enum 的重载版本，不同参数的处理
std::string to_str_enum(en_way en)
{
    switch (en)
    {
    case en_way::e_walk:
    {
        return "步行";
    }
    case en_way::e_bicycle:
    {
        return "单车";
    }
    case en_way::e_busy:
    {
        return "公交";
    }
    case en_way::e_car:
    {
        return "汽车";
    }
    default:
        break;
    }
    return "NULL";
}

// 返回不同类型对应的结构体信息
shopping_check get_discount_remind(en_weekday en)
{
    switch (en)
    {
    case e_monday:
    case e_tuesday:
    case e_wednesday:
    {
        shopping_check ct;
        ct.money = 10;
        ct.time = 20;
        ct.distance = 1;
        ct.way = en_way::e_walk;
        ct.describe = "楼下便利店有冰红茶买一送一活动";
        return ct;
    }
    case e_thursday:
    {
        shopping_check ct;
        ct.money = 50;
        ct.time = 12;
        ct.distance = 5;
        ct.way = en_way::e_busy;
        ct.describe = "肯德基疯狂星期四（晕公交车）";
        return ct;
    }
    case e_friday:
    {
        shopping_check ct;
        ct.money = 15;
        ct.time = 18;
        ct.distance = 3;
        ct.way = en_way::e_bicycle;
        ct.describe = "利民便利店口香糖优惠";
        return ct;
    }
    case e_saturday:
    {
        shopping_check ct;
        ct.money = 300;
        ct.time = 13;
        ct.distance = 20;
        ct.way = en_way::e_car;
        ct.describe = "胖子超市大米打折";
        return ct;
    }
    case e_sunday:
    {
        shopping_check ct;
        ct.money = 25;
        ct.time = 9;
        ct.distance = 1;
        ct.way = en_way::e_walk;
        ct.describe = "熊猫超市香蕉打折";
        return ct;
    }
    default:
        break;
    }

    return {};
}

// 打印结构体信息
void print_shopping_check(en_weekday en)
{
    // 字符串R表达式，所见所得，无需使用 \n 进行换行
    // R表达时会无视字符串的任何转换格式
    std::string str =
        R"(
不要忘记 {0} 的打折活动：{1} 
带钱：{2}块，时间：{3}点，距离：{4}公里，交通：{5}
)";

    // 格式化字符串
    // to_str_enum(ct.way) to_str_enum(en) 调用的是两个不同函数
    shopping_check ct = get_discount_remind(en);
    replace_str_ref(str, "{0}", to_str_enum(en));
    replace_str_ref(str, "{1}", ct.describe);
    replace_str_ref(str, "{2}", std::to_string(ct.money));
    replace_str_ref(str, "{3}", std::to_string(ct.time));
    replace_str_ref(str, "{4}", std::to_string(ct.distance));
    replace_str_ref(str, "{5}", to_str_enum(ct.way));
    std::cout << str << std::endl;
}

int main(int argc, char **argv)
{
    MCLOG("打印每天任务");
    print_shopping_check(e_monday);
    print_shopping_check(e_tuesday);
    print_shopping_check(e_wednesday);
    print_shopping_check(e_thursday);
    print_shopping_check(e_friday);
    print_shopping_check(e_saturday);
    print_shopping_check(e_sunday);

    MCLOG("查看枚举值");
    MCLOG("查看类型 en_weekday");
    for (int i = e_monday; i <= e_sunday; i++)
    {
        MCLOG($(i));
    }

    MCLOG("查看类型 en_way ，需要强转，非常不推荐");
    for (int i = static_cast<int>(en_way::e_walk); i <= static_cast<int>(en_way::e_car); i++)
    {
        MCLOG($(i));
    }

    /*
        // 不好的习惯：
        for (int i = (int)en_way::e_walk; i <= (int)en_way::e_car; i++)
        {
            MCLOG($(i));
        }
    */

    MCLOG("结构体二进制结构");
    struct two_int
    {
        char n1 = 1;
        char n2 = 2;
        char n4 = 4;
        char n8 = 8;
    };

    // 转为等长类型整数后打印二进制
    two_int ct;
    int ibyte;
    std::memcpy(&ibyte, &ct, sizeof(ibyte));
    std::string bit_ct = print_byte<int>(ibyte);

    // char 无法打印数据，需要转类型
    int int32_n1 = ct.n1;
    int int32_n2 = ct.n2;
    int int32_n4 = ct.n4;
    int int32_n8 = ct.n8;
    int two_int_len = sizeof(two_int);

    MCLOG($(int32_n1) $(int32_n2) $(int32_n4) $(int32_n8));
    MCLOG($(ibyte) $(two_int_len));
    MCLOG($(bit_ct));

    return 0;
}