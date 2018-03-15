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

typedef struct _RuleNode
{
	char process_name[16];
	char file_name[128];
	int offset;
	struct _RuleNode* next;
}RuleNode;



typedef struct _RuleComponent
{
	RuleNode* head;
}RuleComponent;

typedef struct _Rule
{
	RuleComponent P;
	RuleComponent Q;
	int support;
	double confidence;
	struct _Rule* next;
}Rule;

typedef struct _PrefetchRules
{
	Rule* head;
}PrefetchRules;


//打印一个规则部件
void printComponent(RuleComponent* P);
//找出一个前件对应的所有后件
RuleComponent* findAllConsequents(PrefetchRules* prefetchRules, char* process_name, char* file_name, int offset);
int sizeOfComponent(RuleComponent* ruleComponent);
//为一个规则前件或者后件添加Node
void addNode(RuleComponent* component, RuleNode *ruleNode);
//为一个规则前件或者后件添加Node
void addNode(RuleComponent* component, char* process_name, char* file_name, int offset);

//打印一条规则
void printARule(Rule* rule);
//读取文件到内存中
//文件格式：
//    weibo temp 111,weixin ttt 222 ==> tencent yyy 678 #sup = 0.5#conf = 0.6
void loadRules(PrefetchRules *prefetchRules, string path);
void printRules(PrefetchRules *prefetchRules);
//添加一项规则
void put(PrefetchRules* prefetchRules, RuleComponent P, RuleComponent Q);
void put(PrefetchRules* prefetchRules, RuleComponent P, RuleComponent Q, int support, double confidence);















//class Rule
//{
//public:
//	vector<unsigned int> P;
//	vector<unsigned int> Q;
//	int support;
//	double confidence;
//
//	Rule() {}
//
//	Rule(vector<unsigned int> P, vector<unsigned int> Q) 
//	{
//		this->P = P;
//		this->Q = Q;
//	}
//
//	Rule(vector<unsigned int> P, vector<unsigned int> Q,int support,double confidence)
//	{
//		this->P = P;
//		this->Q = Q;
//
//		this->support = support;
//		this->confidence = confidence;
//	}
//
//	// 规定对象排序的算法：
//	//		先按照support从大到小排序；
//	//		如果support相等，则按照confidence从大到小排序
//	bool static decrease(const Rule &rule1, const Rule &rule2)
//	{
//		if (rule1.support > rule2.support)
//		{
//			return true;
//		}
//		else if (rule1.support == rule2.support)
//		{
//			return rule1.confidence > rule2.confidence;
//		}
//		return false;
//	}
//
//};

//class PrefetchRules
//{
//private:
//	vector<Rule> rules;
//public:
//	PrefetchRules();
//	~PrefetchRules();
//	
//	//添加一项规则
//	void put(vector<unsigned int> P,vector< unsigned int> Q);
//	void put(vector<unsigned int> P, vector<unsigned int> Q, int support, double confidence);
//
//
//	void pop(unsigned int P);
//	
//	//查找要预取进来的page的集合
//	//
//	vector<unsigned int> findAllToAll(unsigned int P, unordered_map<unsigned, Node *>*,int capacity , double ratio);
//
//
//	// 查找到前件的所有后件
//	vector<unsigned int> findAll(unsigned int P);
//	// 加载规则文件
//	void loadRules(string path);
//	
//	// 找到前件P所在的下标，如果没有就返回-1
//	int findFrontPiece(vector<unsigned int> P);
//
//};
//
