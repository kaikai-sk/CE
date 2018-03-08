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
	double support;
	double confidence;

	Rule() {}

	Rule(vector<unsigned int> P, vector<unsigned int> Q) 
	{
		this->P = P;
		this->Q = Q;
	}
};

class PrefetchRules
{
private:
	vector<Rule> rules;
public:
	PrefetchRules();
	~PrefetchRules();
	void put(vector<unsigned int> P,vector< unsigned int> Q);
	void pop(unsigned int P);
	vector<unsigned int> find(unsigned int P, unordered_map<unsigned, Node *>*);
	// 查找到前件的所有后件
	vector<unsigned int> findAll(unsigned int P);
	// 加载规则文件
	void loadRules(string path);
	// 找到P所在的下标，如果没有就返回-1
	int find(vector<unsigned int> P);
};

