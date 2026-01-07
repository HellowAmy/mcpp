#ifndef MCLOG_H
#define MCLOG_H

// 日志打印
#define MCLOG(value) \
    std::cout << value << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl;

// 日志参数格式化
#define $(value) \
    "[" #value ": " << value << "] "

#endif // MCLOG_H
