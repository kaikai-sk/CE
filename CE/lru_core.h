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
public:

    LRUCore();
	LRUCore(PrefetchRules prefetchRules);

	LRUCore(PrefetchRules prefetchRules, ofstream* ofs_lruCore_snapshoot);

    ~LRUCore();

    unsigned size(void);
    bool exist(unsigned address);
    unsigned update(unsigned address,unsigned tag);
    Node remove(unsigned address);
	void setCapacity(unsigned capacity);

#ifdef __DEBUG_lru_core__
    friend int main(void);
#endif
	void printLRUCoreSnapshoot();
	// 指定打印snapshoot的文件名
	void setOfs(ofstream* ofs_lru_core_snapshoot);
};


#endif
