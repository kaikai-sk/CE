#include "cache.h"
#include "policy.h"


/*
	看是否cache命中
*/
int Cache::query(unsigned address)
{
	vector<int> accessPageNo;

	//地址不在cache中
    if(buffer.find(address) == buffer.end())
    {
        vector<Action*> actionList= policy->miss(address);
		
		for (int i = 0; i < actionList.size(); i++)
		{
			Action action = *actionList[i];
			switch (action.type)
			{
			case POLICY_IGNORE:
				accessPageNo.push_back(-1);
				break;
			case POLICY_APPEND:
				buffer.insert(action.new_address);
				accessPageNo.push_back(action.new_address);
				break;
			case POLICY_REPLACE:
				buffer.erase(action.old_address);
				buffer.insert(action.new_address);
				accessPageNo.push_back(action.new_address);
				this->replaceNum++;
				break;
			case POLICY_PREFETCH_REPLACE:
				buffer.erase(action.prefetch_old_address);
				buffer.insert(action.prefetch_address);
				accessPageNo.push_back(action.prefetch_address);
				this->replaceNum ++;
				break;
			case POLICY_PREFETCH_APPEND:
				buffer.insert(action.prefetch_address);
				accessPageNo.push_back(action.prefetch_address);
				break;
			}
		}

		this->printAccessTrace(accessPageNo);
		this->printCacheSnapshoot();
        return CACHE_MISS;
    }
    else
    {
        vector<Action*> actionList = policy->hit(address);
		for (int i = 0; i < actionList.size(); i++)
		{
			Action action = *actionList[i];
			switch (action.type)
			{
			case POLICY_IGNORE:
				accessPageNo.push_back(-1);
				break;
			case POLICY_APPEND:
				buffer.insert(action.new_address);
				accessPageNo.push_back(action.new_address);
				break;
			case POLICY_REPLACE:
				buffer.erase(action.old_address);
				buffer.insert(action.new_address);
				accessPageNo.push_back(action.new_address);
				this->replaceNum++;
				break;
			case POLICY_PREFETCH_REPLACE:
				buffer.erase(action.prefetch_old_address);
				buffer.insert(action.prefetch_address);
				accessPageNo.push_back(action.prefetch_address);
				this->replaceNum++;
				break;
			case POLICY_PREFETCH_APPEND:
				//buffer.insert(action.new_address);
				buffer.insert(action.prefetch_address);
				accessPageNo.push_back(action.prefetch_address);
				break;
			}
		}

		this->printAccessTrace(accessPageNo);
		this->printCacheSnapshoot();
        return CACHE_HIT;
    }
}


#ifdef __DEBUG_cache__
// g++ -D__DEBUG_cache__ -Wall cache.cpp lru_policy.cpp
#include <iostream>
#include "lru_policy.h"
using namespace std;
int main(void)
{
    unsigned stream[] = {0, 1, 2, 3, 4, 5, 6, 5, 6, 7, 6, 5, 4, 3, 2 ,1, 0};
    unsigned length = sizeof(stream) / sizeof(unsigned);

    Policy *policy = new LRUPolicy(5);
    Cache *cache = new Cache(5, policy);

    unsigned hit_count = 0;
    unsigned miss_count = 0;

    for(unsigned i = 0; i < length; i++)
    {
        if(cache->query(stream[i]) == CACHE_HIT)
            hit_count++;
        else
            miss_count++;
    }

    cout << "hit:" << hit_count << " / miss:" << miss_count << endl;

    delete cache;
    delete policy;

    return 0;
}
#endif
