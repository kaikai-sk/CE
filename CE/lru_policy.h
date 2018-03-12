#ifndef HEADER_LRU_POLICY
#define HEADER_LRU_POLICY


#include <unordered_map>
#include "policy.h"
#include "lru_core.h"
#include "PrefetchRules.h"
#include "Node.h"
#include <fstream>
#include <vector>

class LRUPolicy: public Policy
{
private:
	//预取次数
	unsigned prefetchCount;
	//预取替换次数
	unsigned prefetchReplaceCount;

    unsigned capacity;
	//预取规则
	PrefetchRules prefetchRules;

    LRUCore lru_core;
    /*Action action;*/
	//打印预取的踪迹
	ofstream* ofs;
	ofstream* ofs_lru_core_snapshoot;
	ofstream* ofs_page_detail;

public:
    LRUPolicy(unsigned capacity);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules,ofstream* ofs);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules, ofstream* ofs,ofstream* ofs_lru_core_snapshoot);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules, ofstream* ofs, ofstream* ofs_lru_core_snapshoot,ofstream* ofs_page_detail);
    ~LRUPolicy();

    void hit(unsigned address);
	void miss(unsigned address);

	// 打印出预取的trace
	void printAccessTrace(unsigned address);
	unsigned getPrefetchCount();
	unsigned getPrefetchReplaceCount();
	// 统计是否预取与命中数目等信息
	void statistic();

	// 查找一个page是不是在cache中
	int findPage(unsigned int pageNo);
	// 进行预取操作
	void doPrefetch(int address);
};


#endif
