#ifndef HEADER_CACHE
#define HEADER_CACHE


#include <unordered_set>
#include "policy.h"
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
	//������
    std::unordered_set<unsigned> buffer;
    //����Ĳ���
	Policy *policy;
	//��ӡ��¼���ļ�
	ofstream* ofs;
	//��ӡcache���յ��ļ�
	ofstream* ofs_cache_snapshoot;

public:
    Cache(unsigned capacity, Policy *policy)
        :capacity(capacity), policy(policy) 
	{
		this->replaceNum = 0;
		this->buffer.reserve(capacity);
	}

	Cache(unsigned capacity, Policy *policy, ofstream* ofs)
		:capacity(capacity), policy(policy),ofs(ofs)
	{
		this->replaceNum = 0;
		this->buffer.reserve(capacity);
	}

	Cache(unsigned capacity, Policy *policy, ofstream* ofs,ofstream* ofs_cache_snapshoot)
		:capacity(capacity), policy(policy), ofs(ofs),ofs_cache_snapshoot(ofs_cache_snapshoot)
	{
		this->replaceNum = 0;
		this->buffer.reserve(capacity);
	}

    int query(unsigned address);

	unsigned getReplaceNum()
	{
		return this->replaceNum;
	}

	// ���page no���ļ��У�������¼trace
	void printAccessTrace(int pageNo)
	{
		(*ofs) << pageNo << endl;
	}

	// ��ӡtrace,������ӡ���
	void printAccessTrace(vector<int> pageNos)
	{
		for (int i = 0; i < pageNos.size(); i++)
		{
			(*ofs) << pageNos[i] << " ";
		}
		(*ofs) << endl;
	}

	// ��ӡ��ǰcache�Ŀ���
	void printCacheSnapshoot()
	{
		for (unordered_set<unsigned>::iterator i = this->buffer.begin(); i != this->buffer.end(); i++) 
		{
			(*ofs_cache_snapshoot) << *i << ' ';
		}
		(*ofs_cache_snapshoot) << endl;
	}
};


#endif
