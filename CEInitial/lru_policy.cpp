#include "lru_policy.h"


LRUPolicy::LRUPolicy(unsigned capacity)
    : capacity(capacity)
{
}

LRUPolicy::~LRUPolicy()
{
}

Action &LRUPolicy::hit(unsigned address)
{
    lru_core.update(address);

    action.type = POLICY_IGNORE;
    return action;
}

Action &LRUPolicy::miss(unsigned address)
{
    unsigned old_address;
    unsigned length = lru_core.size();

    if(length == capacity)
    {
        old_address = lru_core.remove(-1);
        lru_core.update(address);

        action.type = POLICY_REPLACE;
        action.old_address = old_address;
        action.new_address = address;

        return action;
    }
    else  // (length < this->capacity)
    {
        lru_core.update(address);

        action.type = POLICY_APPEND;
        action.new_address = address;
        return action;
    }
}


#ifdef __DEBUG_lru_policy__
// g++ -D__DEBUG_lru_policy__ -Wall lru_core.cpp lru_policy.cpp
#include <iostream>
using namespace std;
int main(void)
{
    LRUPolicy lp(5);

    for(int i = 0; i < 10; i++)
    {
        Action &action = lp.miss(i);
        cout << action.type << "-"
             << action.old_address << "-"
             << action.new_address << endl;
    }

    for(int i = 9; i > 4; i--)
    {
        Action &action = lp.hit(i);
        cout << action.type << "-"
             << action.old_address << "-"
             << action.new_address << endl;
    }

    return 0;
}
#endif
