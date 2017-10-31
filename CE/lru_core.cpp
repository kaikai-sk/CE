#include "lru_core.h"


class Node
{
public:
	//指向下一个节点
    Node    *next;
	//指向前一个节点
    Node    *prev;
	//page No
    unsigned key;

public:
    Node(): next(0), prev(0), key(0) {}

    void attach(Node* prev);
    void detach();
};

/*
	链表中插入一个节点
*/
void Node::attach(Node *head)
{
    if(head->next)
        head->next->prev = this;
    this->next = head->next;

    head->next = this;
    this->prev = head;
}

/*
链表中删除一个节点
*/
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

LRUCore::LRUCore(PrefetchRules prefetchRules)
{
	LRUCore();
	this->prefetchRules = prefetchRules;
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

///*
//	LRU 工作的地方
//*/
//unsigned LRUCore::update(unsigned address)
//{
//    Node *node;
//    const auto &iter = records.find(address);
//
//	unsigned prefetchAddress = this->prefetchRules.find(address);
//	Node* prefetchedNode;
//
//	if (this->size() < capacity)
//	{
//		//所访问的地址不在cache中，添加一个新的页面
//		if (iter == records.end())
//		{
//			node = new Node;
//
//			node->key = address;
//			node->attach(head);
//			records[address] = node;
//
//			//这里添加预取页面,添加到最近使用的页面
//			/*	const auto &iterNew = records.find(prefetchAddress);
//				if (this->size()<this->capacity && iterNew == records.end())
//				{
//					prefetchedNode->key = prefetchAddress;
//					prefetchedNode->attach(head);
//					records[prefetchAddress] = prefetchedNode;
//				}
//				else if (this->size() == capacity  && iterNew == records.end())
//				{
//					this->remove(-1);
//					prefetchedNode->key = prefetchAddress;
//					prefetchedNode->attach(head);
//					records[prefetchAddress] = prefetchedNode;
//				}*/
//
//		}
//		//所访问的地址已经在cache中了
//		else
//		{
//			node = iter->second;
//			node->detach();
//			node->attach(head);
//		}
//	}
//	else if (this->size() == capacity)
//	{
//		//所访问的地址不在cache中，需要替换掉一个页面
//		if (iter == records.end())
//		{
//			//删除最久的那一个页面
//			this->remove(-1);
//			
//			node->key = address;
//			node->attach(head);
//			records[address] = node;
//		}
//		//如果访问的address在cache中，就需要调整下顺序
//		else
//		{
//			node = iter->second;
//			node->detach();
//			node->attach(head);
//		}
//	}
//
//	
//	//LRU替换策略,所访问的address已经在cache里面了。
//    else
//    {
//		//访问的是一个新的节点，并且容量已将达到了capacity，需要删除最久访问的那个
//		if (this->size() == this->capacity)
//		{
//			this->remove(-1);
//			node->attach(head);
//		}
//		else
//		{
//			node = iter->second;
//			node->detach();
//			node->attach(head);
//		}
//    }
//
//    return address;
//}

unsigned LRUCore::update(unsigned address)
{
	Node *node;
	const auto &iter = records.find(address);

	if (iter == records.end())
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

void LRUCore::setCapacity(unsigned capacity)
{
	this->capacity = capacity;
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
