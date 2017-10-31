#ifndef HEADER_LRU_CORE
#define HEADER_LRU_CORE


#include <unordered_map>
#include "PrefetchRules.h"

class Node;


class LRUCore
{
    Node    *head;
    Node    *tail;
    std::unordered_map<unsigned, Node *> records;
	unsigned capacity;

	//保存一张预取规则表
	PrefetchRules prefetchRules;

public:
    LRUCore();
	LRUCore(PrefetchRules prefetchRules);
    ~LRUCore();

    unsigned size(void);
    bool exist(unsigned address);
    unsigned update(unsigned address);
    unsigned remove(unsigned address);
	void setCapacity(unsigned capacity);

#ifdef __DEBUG_lru_core__
    friend int main(void);
#endif
};


#endif
