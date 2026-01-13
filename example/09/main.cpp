#include <iostream>

#include "mclog.h"
#include "format.h"

// 最基本的动物类
class animal
{
public:
    // 公开区域，可让任何作用域访问
    // 默认无参数构造函数
    animal() {}

    // 有参数默构造函数，主要用于给数据赋值
    animal(bool male, size_t age)
        : _male(male), _age(age) {}

    // 纯虚函数，出现纯虚函数之后这个类将无法创建对象
    // 指定自己为特定动物类型，需要具体的动物类型子类实现，现在只是占位声明
    virtual std::string kind() = 0;

    // 这是获取类内数据的公开函数
    bool is_male()
    {
        return _male;
    }
    size_t get_age()
    {
        return _age;
    }
    std::string DNA()
    {
        return _DNA;
    }

    // 注意这里如果只有获取没有设置，表明数据一但确定无法更改
    // 类内参数只能通过公开函数的接口进行设置
    // 提供变性设置
    void set_male(bool male)
    {
        _male = male;
    }

protected:
    // 保护区域，只有本身和子类可以访问
    // 可改变的-这个继承类不一致的内容
    bool _male = true;
    size_t _age = 0;

private:
    // 私有区域，只有本身可以访问
    // 动物的底层代码
    std::string _DNA = "DNA_ANIMAL_EAT_SLEEP_SEX";
};

// 继承 animal 的子类，需要指定自己的 kind 种类
// 这个类存在 action 虚函数，无法创建对象
// 这个类有自己的 _name 类型，子类可用，与继承 animal 的数据结构不一致
class rabbit : public animal
{
public:
    // 继承父类的构造函数，等于自己也拥有了父类的构造函数
    using animal::animal;

    // 构造函数，自己的带参数构造函数，相同参数下，优先等级比父类高
    rabbit(bool male, size_t age, std::string name)
        : animal(male, age), _name(name) {}

    // 纯虚函数，需要子类实现
    virtual std::string action() = 0;

    // 实现父类的纯虚函数
    // 表明自己的类型是兔子
    std::string kind() override
    {
        return "REBBIT";
    }

    // introduce 本身信息是不完整的，需要使用 _name 和 action()
    // action() 是虚函数，需要子类实现才行
    std::string introduce()
    {
        std::string str = "我是 {0} 接下来我要 {1}\n";
        replace_str_ref(str, "{0}", _name);
        replace_str_ref(str, "{1}", action());
        return str;
    };

    // 给外部获取 name 数据的接口
    std::string get_name()
    {
        return _name;
    }

protected:
    std::string _name;
};

// 继承 animal 的子类，需要指定自己的 kind 种类
class fox : public animal
{
public:
    using animal::animal;
    std::string kind() override
    {
        return "FOX";
    }

    // 这个 show 是虚函数，但不是纯虚函数，可以创建对象
    virtual std::string show()
    {
        return "野生狐狸不会表演";
    };
};

// rabbit 的子类，需要实现 action 虚函数
// 存在继承构造函数可在外部使用
// 本身实现构造函数，优先实现本身构造
class JudyHopps : public rabbit
{
public:
    // 继承构造函数
    using rabbit::rabbit;

    // 自己的构造函数
    JudyHopps(int age)
        : rabbit(false, age, "朱迪") {}

    // 实现虚函数功能
    // 指定不同年龄干不一样的事情
    std::string action() override
    {
        if (_age > 18)
        {
            return "打击动物城犯罪分子";
        }
        else if (_age > 10)
        {
            return "帖200张罚单";
        }
        return "在家种胡萝卜";
    }
};

// rabbit 的子类，需要实现 action 虚函数
class BonnieHopps : public rabbit
{
public:
    BonnieHopps()
        : rabbit(false, 55, "朱迪妈妈") {}

    std::string action() override
    {
        return "和朱迪的爸爸在家种胡萝卜";
    }
};

// rabbit 的子类，需要实现 show 虚函数
class NickWilde : public fox
{
public:
    // 只有一个无参数的构造函数
    // 没有继承父类构造函数，不能通过构造函数的参数赋值
    NickWilde()
    {
        _male = true;
        _age = 25;
    }

    std::string show() override
    {
        return "我叫尼克狐，接下来我会让你知道什么叫一口一个兔兔头";
    }
};

// 这个函数使用了父类指针 animal，这个父类存在虚函数，这里会发生多态调用
// 获取最顶层父类信息
std::string animal_info(animal *actor)
{
    // 根据 is_male 函数的值确认性别
    std::string male = "男";
    if (actor->is_male() == false)
    {
        male = "女";
    }

    // 这里在查看类有关 animal 以及子类的基本信息
    std::string str = "演员信息 "
                      "种类：{0} "
                      "性别：{1} "
                      "年龄：{2} "
                      "原始属性：{3} ";
    replace_str_ref(str, "{0}", actor->kind());
    replace_str_ref(str, "{1}", male);
    replace_str_ref(str, "{2}", std::to_string(actor->get_age()));
    replace_str_ref(str, "{3}", actor->DNA());
    return str;
}

// 可多态调用 fox 类虚函数与成员函数
// 接收和 fox 类型相关的数据，包括 fox 类型，以及他的子类
// show 函数是虚函数，这里会调用子类的 show 函数
void showtime_fox(fox *actor)
{
    std::string str = "{0}\n少废话，请开始你的表演\n{1}";
    replace_str_ref(str, "{0}", animal_info(actor));
    replace_str_ref(str, "{1}", actor->show());
    std::cout << str << std::endl;
}

// 可多态调用 rabbit 类虚函数与成员函数
// action 函数是虚函数，子类实现
// 但是这里并不调用 action，而是调用了 introduce 函数来间接调用 action 虚函数
void showtime_rabbit(rabbit *actor)
{
    std::string str = "{0}\n这里是兔子胡萝卜节表演大会，请 {1} 进行自我介绍\n{2}";
    replace_str_ref(str, "{0}", animal_info(actor));
    replace_str_ref(str, "{1}", actor->get_name());
    replace_str_ref(str, "{2}", actor->introduce());
    std::cout << str << std::endl;
}

int main(int argc, char **argv)
{
    MCLOG("朱迪小时候");
    JudyHopps jd5 = JudyHopps(5);
    showtime_rabbit(&jd5);

    MCLOG("朱迪刚入职");
    JudyHopps jd15 = JudyHopps(15);
    showtime_rabbit(&jd15);

    MCLOG("朱迪组搭档");
    JudyHopps jd20 = JudyHopps(20);
    showtime_rabbit(&jd20);

    MCLOG("朱迪的妈妈");
    BonnieHopps jdmm = BonnieHopps();
    showtime_rabbit(&jdmm);

    MCLOG("尼克的表演");
    NickWilde nick = NickWilde();
    showtime_fox(&nick);

    MCLOG("野生的狐狸");
    fox nono = fox(true, 4);
    showtime_fox(&nono);

    return 0;
}