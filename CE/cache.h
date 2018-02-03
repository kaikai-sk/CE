#ifndef HEADER_CACHE
#define HEADER_CACHE


#include <unordered_set>
#include "policy.h"
#include <fstream>
using namespace std;


#define CACHE_HIT  1
#define CACHE_MISS 0

//这是一个cache的模拟器
class Cache
{
private:
	unsigned replaceNum;
	//缓存容量
    unsigned capacity;
	//缓冲区
    std::unordered_set<unsigned> buffer;
    //缓存的策略
	Policy *policy;
	//打印记录的文件
	ofstream* ofs;
	//打印cache快照的文件
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

	// 输出page no到文件中，即，记录trace
	void printAccessTrace(int pageNo)
	{
		(*ofs) << pageNo << endl;
	}

	// 打印trace,连续打印多个
	void printAccessTrace(vector<int> pageNos)
	{
		for (int i = 0; i < pageNos.size(); i++)
		{
			(*ofs) << pageNos[i] << " ";
		}
		(*ofs) << endl;
	}

	// 打印当前cache的快照
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
