#ifndef HEADER_CACHE
#define HEADER_CACHE


#include <unordered_set>
#include "policy.h"


#define CACHE_HIT  1
#define CACHE_MISS 0


class Cache
{
private:
	unsigned replaceNum;
	//»º´æÈÝÁ¿
    unsigned capacity;
	//»º³åÇø
    std::unordered_set<unsigned> buffer;
    //»º´æµÄ²ßÂÔ
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
