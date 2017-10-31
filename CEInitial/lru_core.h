#ifndef HEADER_LRU_CORE
#define HEADER_LRU_CORE


#include <unordered_map>


class Node;


class LRUCore
{
    Node    *head;
    Node    *tail;
    std::unordered_map<unsigned, Node *> records;
public:
    LRUCore();
    ~LRUCore();

    unsigned size(void);
    bool exist(unsigned address);
    unsigned update(unsigned address);
    unsigned remove(unsigned address);

#ifdef __DEBUG_lru_core__
    friend int main(void);
#endif
};


#endif
