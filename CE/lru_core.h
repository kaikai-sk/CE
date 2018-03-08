#ifndef HEADER_LRU_CORE
#define HEADER_LRU_CORE


#include <unordered_map>
#include <iostream>
#include <fstream>
#include "PrefetchRules.h"
#include "Node.h"

using namespace std;


class LRUCore
{
    Node    *head;
    Node    *tail;
    std::unordered_map<unsigned, Node *> records;
	unsigned capacity;

	//保存一张预取规则表
	PrefetchRules prefetchRules;
	//文件指针，打印cache快照的文件指针
	ofstream* ofs_lruCore_snapshoot;
	//保存一个是否预取，是否命中的矩阵
	int staticMatrix[2][2];
	//预取的节点列表
	vector<Node*> prefetchedNodes;

public:

    LRUCore();
	LRUCore(PrefetchRules prefetchRules);

	LRUCore(PrefetchRules prefetchRules, ofstream* ofs_lruCore_snapshoot);

    ~LRUCore();

    unsigned size(void);
    bool exist(unsigned address);
    unsigned update(unsigned address, PageState pageState);
    Node remove(unsigned address);
	void setCapacity(unsigned capacity);

	void printLRUCoreSnapshoot();
	// 指定打印snapshoot的文件名
	void setOfs(ofstream* ofs_lru_core_snapshoot);
	// 统计预取情况和命中情况
	void statistic();
	// 拿到当前cache的快照
	unordered_map<unsigned, Node *>* getCacheShot();

#ifdef __DEBUG_lru_core__
    friend int main(void);
#endif

};


#endif
