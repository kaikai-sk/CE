#ifndef HEADER_LRU_POLICY
#define HEADER_LRU_POLICY


#include <unordered_map>
#include "policy.h"
#include "lru_core.h"
#include "PrefetchRules.h"
#include <fstream>

class LRUPolicy: public Policy
{
private:
    unsigned capacity;

	PrefetchRules prefetchRules;

    LRUCore lru_core;
    Action action;
	ofstream* ofs;
public:
    LRUPolicy(unsigned capacity);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules,ofstream* ofs);
    ~LRUPolicy();

    Action &hit(unsigned address);
    Action &miss(unsigned address);

#ifdef __DEBUG_lru_policy__
    friend int main(void);
#endif
	// 打印出预取的trace
	void printAccessTrace(unsigned address);
};


#endif
