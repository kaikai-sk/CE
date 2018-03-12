#ifndef HEADER_CACHE
#define HEADER_CACHE


#include <unordered_set>
#include "policy.h"
#include "lru_policy.h"
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
    //缓存的策略
	Policy *policy;
	//打印记录的文件
	ofstream* ofs;
	//打印cache快照的文件
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
	// 输出page no到文件中，即，记录trace
	void printAccessTrace(int pageNo);
	// 打印trace,连续打印多个
	void printAccessTrace(vector<int> pageNos);
	
	// 查找一个page是不是在cache当中
	int findPage(unsigned int address);

	// 打印当前cache的快照
	void printCacheSnapshoot()
	{

	}


};


#endif
