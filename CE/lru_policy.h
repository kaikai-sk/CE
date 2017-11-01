#ifndef HEADER_LRU_POLICY
#define HEADER_LRU_POLICY


#include <unordered_map>
#include "policy.h"
#include "lru_core.h"
#include "PrefetchRules.h"
#include <fstream>
#include <vector>

class LRUPolicy: public Policy
{
private:
	//Ԥȡ����
	unsigned prefetchCount;
	//Ԥȡ�滻����
	unsigned prefetchReplaceCount;

    unsigned capacity;
	//Ԥȡ����
	PrefetchRules prefetchRules;

    LRUCore lru_core;
    /*Action action;*/
	//��ӡԤȡ���ټ�
	ofstream* ofs;
public:
    LRUPolicy(unsigned capacity);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules);
	LRUPolicy(unsigned capacity, PrefetchRules prefetchRules,ofstream* ofs);
    ~LRUPolicy();

    vector<Action*> hit(unsigned address);
	vector<Action*> miss(unsigned address);

#ifdef __DEBUG_lru_policy__
    friend int main(void);
#endif
	// ��ӡ��Ԥȡ��trace
	void printAccessTrace(unsigned address);
	unsigned getPrefetchCount();
	unsigned getPrefetchReplaceCount();
};


#endif
