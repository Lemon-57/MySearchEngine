#ifndef _EDITDISTANCE_HH_
#define _EDITDISTANCE_HH_

#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
//1. 求取一个字符占据的字节数
size_t nBytesCode(const char ch);

//2. 求取一个字符串的字符长度
std::size_t length(const string &str);

//3. 中英文通用的最小编辑距离算法
int editDistance(const string & lhs, const string &rhs);

#endif
