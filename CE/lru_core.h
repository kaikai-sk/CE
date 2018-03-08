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

	//����һ��Ԥȡ�����
	PrefetchRules prefetchRules;
	//�ļ�ָ�룬��ӡcache���յ��ļ�ָ��
	ofstream* ofs_lruCore_snapshoot;
	//����һ���Ƿ�Ԥȡ���Ƿ����еľ���
	int staticMatrix[2][2];
	//Ԥȡ�Ľڵ��б�
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
	// ָ����ӡsnapshoot���ļ���
	void setOfs(ofstream* ofs_lru_core_snapshoot);
	// ͳ��Ԥȡ������������
	void statistic();
	// �õ���ǰcache�Ŀ���
	unordered_map<unsigned, Node *>* getCacheShot();

#ifdef __DEBUG_lru_core__
    friend int main(void);
#endif

};


#endif
