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

