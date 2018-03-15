#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class StringUtil
{
public:
	StringUtil();
	~StringUtil();

	void StringUtil::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& separator);
};

// 将str字符以spl分割,存于dst中，并返回子字符串数量
int split(char dst[][80], char* str, const char* spl);


