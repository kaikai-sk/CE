#ifndef HEADER_CACHE
#define HEADER_CACHE


#include <unordered_set>
#include "policy.h"
#include "lru_policy.h"
#include <fstream>
using namespace std;


#define CACHE_HIT  1
#define CACHE_MISS 0

//����һ��cache��ģ����
class Cache
{
private:
	unsigned replaceNum;
	//��������
    unsigned capacity;
    //����Ĳ���
	Policy *policy;
	//��ӡ��¼���ļ�
	ofstream* ofs;
	//��ӡcache���յ��ļ�
	ofstream* ofs_cache_snapshoot;

public:
	Cache(unsigned capacity, Policy *policy, ofstream* ofs, ofstream* ofs_cache_snapshoot)
		:capacity(capacity), policy(policy), ofs(ofs), ofs_cache_snapshoot(ofs_cache_snapshoot)
	{
		this->replaceNum = 0;
	}

	Cache(unsigned capacity, Policy *policy)
		: capacity(capacity), policy(policy)
	{
		this->replaceNum = 0;
	}

	Cache(unsigned capacity, Policy *policy, ofstream* ofs)
		: capacity(capacity), policy(policy), ofs(ofs)
	{
		this->replaceNum = 0;
	}

    int query(unsigned address);

	unsigned getReplaceNum();
	// ���page no���ļ��У�������¼trace
	void printAccessTrace(int pageNo);
	// ��ӡtrace,������ӡ���
	void printAccessTrace(vector<int> pageNos);
	
	// ����һ��page�ǲ�����cache����
	int findPage(unsigned int address);

	// ��ӡ��ǰcache�Ŀ���
	void printCacheSnapshoot()
	{

	}


};


#endif
