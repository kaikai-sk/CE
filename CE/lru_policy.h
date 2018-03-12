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

	// ��ӡ��Ԥȡ��trace
	void printAccessTrace(unsigned address);
	unsigned getPrefetchCount();
	unsigned getPrefetchReplaceCount();
	// ͳ���Ƿ�Ԥȡ��������Ŀ����Ϣ
	void statistic();

	// ����һ��page�ǲ�����cache��
	int findPage(unsigned int pageNo);
	// ����Ԥȡ����
	void doPrefetch(int address);
};


#endif
