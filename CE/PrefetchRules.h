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

	// �涨����������㷨��
	//		�Ȱ���support�Ӵ�С����
	//		���support��ȣ�����confidence�Ӵ�С����
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
	
	//���һ�����
	void put(vector<unsigned int> P,vector< unsigned int> Q);
	void put(vector<unsigned int> P, vector<unsigned int> Q, int support, double confidence);


	void pop(unsigned int P);
	
	//����ҪԤȡ������page�ļ���
	//
	vector<unsigned int> findAllToAll(unsigned int P, unordered_map<unsigned, Node *>*,int capacity , double ratio);


	// ���ҵ�ǰ�������к��
	vector<unsigned int> findAll(unsigned int P);
	// ���ع����ļ�
	void loadRules(string path);
	
	// �ҵ�ǰ��P���ڵ��±꣬���û�оͷ���-1
	int findFrontPiece(vector<unsigned int> P);

};

