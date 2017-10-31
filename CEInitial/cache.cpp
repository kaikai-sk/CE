#include "cache.h"
#include "policy.h"


int Cache::query(unsigned address)
{
    if(buffer.find(address) == buffer.end())
    {
        Action &action = policy->miss(address);
        switch(action.type)
        {
            case POLICY_IGNORE:
                break;
            case POLICY_APPEND:
                buffer.insert(action.new_address);
                break;
            case POLICY_REPLACE:
                buffer.erase(action.old_address);
                buffer.insert(action.new_address);
				this->replaceNum++;
                break;
        }
        return CACHE_MISS;
    }
    else
    {
        Action &action = policy->hit(address);
        switch(action.type)
        {
            case POLICY_IGNORE:
                break;
            case POLICY_APPEND:
                buffer.insert(action.new_address);
                break;
            case POLICY_REPLACE:
                buffer.erase(action.old_address);
                buffer.insert(action.new_address);
				this->replaceNum++;
                break;
        }
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
