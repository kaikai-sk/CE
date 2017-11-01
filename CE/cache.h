#ifndef HEADER_CACHE
#define HEADER_CACHE


#include <unordered_set>
#include "policy.h"


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
public:
    Cache(unsigned capacity, Policy *policy)
        :capacity(capacity), policy(policy) 
	{
		this->replaceNum = 0;
	}

    int query(unsigned address);

	unsigned getReplaceNum()
	{
		return this->replaceNum;
	}
};


#endif
