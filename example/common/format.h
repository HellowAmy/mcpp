#ifndef FORMAT_H
#define FORMAT_H

#include <string>

// 替换字符串，用于格式化字符串
// 在 org 字符串内，使用 str_new 替换 str_old 字符
// org : 原始内容，会被修改
// str_old : 被替换的字符
// str_new : 新字符串
void replace_str_ref(std::string &org, const std::string &str_old, const std::string &str_new);

#endif // FORMAT_H
