#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class SetUtil
{
public:
	SetUtil();
	~SetUtil();

	bool isSubset(vector<unsigned int> v1, vector<unsigned int> v2);
	//将V2的内容加到V1中
	void setAppend(vector<unsigned int>& v1, vector<unsigned int>& v2);


	// 判断两个集合是否完全相同
	bool isEqual(vector<unsigned int> P, vector<unsigned int> Q);
};

