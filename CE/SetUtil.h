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
	//��V2�����ݼӵ�V1��
	void setAppend(vector<unsigned int>& v1, vector<unsigned int>& v2);


	// �ж����������Ƿ���ȫ��ͬ
	bool isEqual(vector<unsigned int> P, vector<unsigned int> Q);
};

