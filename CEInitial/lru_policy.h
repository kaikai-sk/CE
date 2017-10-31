#ifndef HEADER_LRU_POLICY
#define HEADER_LRU_POLICY


#include <unordered_map>
#include "policy.h"
#include "lru_core.h"


class LRUPolicy: public Policy
{
private:
    unsigned capacity;

    LRUCore lru_core;
    Action action;
public:
    LRUPolicy(unsigned capacity);
    ~LRUPolicy();

    Action &hit(unsigned address);
    Action &miss(unsigned address);

#ifdef __DEBUG_lru_policy__
    friend int main(void);
#endif
};


#endif
