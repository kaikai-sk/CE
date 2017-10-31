#include "lru_core.h"


class Node
{
public:
    Node    *next;
    Node    *prev;
    unsigned key;

public:
    Node(): next(0), prev(0), key(0) {}

    void attach(Node* prev);
    void detach();
};

void Node::attach(Node *head)
{
    if(head->next)
        head->next->prev = this;
    this->next = head->next;

    head->next = this;
    this->prev = head;
}

void Node::detach()
{
    if(this->prev)
        this->prev->next = this->next;
    if(this->next)
        this->next->prev = this->prev;

    this->next = 0;  // I don't like `nullptr`
    this->prev = 0;
}


LRUCore::LRUCore()
    : head(new Node), tail(new Node)
{
    tail->attach(head);
}

LRUCore::~LRUCore()
{
    for(Node *node = head, *temp; node; node = temp)
    {
        temp = node->next;
        delete node;
    }
}

unsigned LRUCore::size(void)
{
    return records.size();
}

bool LRUCore::exist(unsigned address)
{
    return records.find(address) != records.end();
}

unsigned LRUCore::update(unsigned address)
{
    Node *node;
    const auto &iter = records.find(address);

    if(iter == records.end())
    {
        node = new Node;

        node->key = address;
        node->attach(head);
        records[address] = node;
    }
    else
    {
        node = iter->second;

        node->detach();
        node->attach(head);
    }

    return address;
}

unsigned LRUCore::remove(unsigned address)
/* 'address == -1' means remove the last one */
{
    Node *node;

    if(address == (unsigned)-1)
    {
        node = tail->prev;

        node->detach();
        records.erase(node->key);

        address = node->key;
    }
    else
    {
        const auto &iter = records.find(address);
        node = iter->second;

        node->detach();
        records.erase(node->key);
    }

    delete node;
    return address;
}


#ifdef __DEBUG_lru_core__
// g++ -D__DEBUG_lru_core__ -Wall lru_core.cpp
#include <iostream>
using namespace std;
int main(void)
{
    LRUCore lru_core;

    for(unsigned i = 0; i < 5; i++)
    {
        lru_core.update(i);
    }
    for(unsigned i = 5; i < 10; i++)
    {
        lru_core.remove(-1);
        lru_core.update(i);
    }
    for(Node *node = lru_core.head->next;
        node != lru_core.tail;
        node = node->next)
    {
        cout << node->key << " ";
    }
    cout << endl;

    for(unsigned i = 9; i > 4; i--)
    {
        lru_core.update(i);
    }
    for(Node *node = lru_core.head->next;
        node != lru_core.tail;
        node = node->next)
    {
        cout << node->key << " ";
    }
    cout << endl;

    return 0;
}
#endif
