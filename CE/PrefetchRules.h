#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "StringUtil.h"
#include "Node.h"
#include "SetUtil.h"
using namespace std;

class Rule
{
public:
	vector<unsigned int> P;
	vector<unsigned int> Q;
	int support;
	double confidence;

	Rule() {}

	Rule(vector<unsigned int> P, vector<unsigned int> Q) 
	{
		this->P = P;
		this->Q = Q;
	}

	Rule(vector<unsigned int> P, vector<unsigned int> Q,int support,double confidence)
	{
		this->P = P;
		this->Q = Q;

		this->support = support;
		this->confidence = confidence;
	}

	// 规定对象排序的算法：
	//		先按照support从大到小排序；
	//		如果support相等，则按照confidence从大到小排序
	bool static decrease(const Rule &rule1, const Rule &rule2)
	{
		if (rule1.support > rule2.support)
		{
			return true;
		}
		else if (rule1.support == rule2.support)
		{
			return rule1.confidence > rule2.confidence;
		}
		return false;
	}

};

class PrefetchRules
{
private:
	vector<Rule> rules;
public:
	PrefetchRules();
	~PrefetchRules();
	
	//添加一项规则
	void put(vector<unsigned int> P,vector< unsigned int> Q);
	void put(vector<unsigned int> P, vector<unsigned int> Q, int support, double confidence);


	void pop(unsigned int P);
	
	//查找要预取进来的page的集合
	//
	vector<unsigned int> findAllToAll(unsigned int P, unordered_map<unsigned, Node *>*,int capacity , double ratio);


	// 查找到前件的所有后件
	vector<unsigned int> findAll(unsigned int P);
	// 加载规则文件
	void loadRules(string path);
	
	// 找到前件P所在的下标，如果没有就返回-1
	int findFrontPiece(vector<unsigned int> P);

};

