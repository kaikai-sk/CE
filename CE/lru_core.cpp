#include "lru_core.h"






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

LRUCore::LRUCore(PrefetchRules prefetchRules, ofstream* ofs_lruCore_snapshoot)
{
	LRUCore();
	this->prefetchRules = prefetchRules;
	this->ofs_lruCore_snapshoot = ofs_lruCore_snapshoot;
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
//	LRU �����ĵط�
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
//		//�����ʵĵ�ַ����cache�У����һ���µ�ҳ��
//		if (iter == records.end())
//		{
//			node = new Node;
//
//			node->key = address;
//			node->attach(head);
//			records[address] = node;
//
//			//�������Ԥȡҳ��,��ӵ����ʹ�õ�ҳ��
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
//		//�����ʵĵ�ַ�Ѿ���cache����
//		else
//		{
//			node = iter->second;
//			node->detach();
//			node->attach(head);
//		}
//	}
//	else if (this->size() == capacity)
//	{
//		//�����ʵĵ�ַ����cache�У���Ҫ�滻��һ��ҳ��
//		if (iter == records.end())
//		{
//			//ɾ����õ���һ��ҳ��
//			this->remove(-1);
//			
//			node->key = address;
//			node->attach(head);
//			records[address] = node;
//		}
//		//������ʵ�address��cache�У�����Ҫ������˳��
//		else
//		{
//			node = iter->second;
//			node->detach();
//			node->attach(head);
//		}
//	}
//
//	
//	//LRU�滻����,�����ʵ�address�Ѿ���cache�����ˡ�
//    else
//    {
//		//���ʵ���һ���µĽڵ㣬���������ѽ��ﵽ��capacity����Ҫɾ����÷��ʵ��Ǹ�
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

/*
	��ӡcache�Ŀ���
*/
void LRUCore::printLRUCoreSnapshoot()
{
	for (auto &iter = records.begin(); iter != records.end(); iter++)
	{
		(*this->ofs_lruCore_snapshoot) << iter->first << ' ';
	}
	(*this->ofs_lruCore_snapshoot) << endl;
}

unsigned LRUCore::update(unsigned address,unsigned tag)
{
	Node *node;
	const auto &iter = records.find(address);

	/*���ǲ�������*/
	if (iter == records.end())
	{
		node = new Node;
		if (tag == 1)
		{
			node->tag = 1;
		}
		node->key = address;
		node->attach(head);
		records[address] = node;
	}

	//�������е������Ҫ����ջ��˳��
	else
	{
		node = iter->second;

		//Ԥȡ�Ŀ鱻���е����
		if (node->tag == 1)
		{
			//���д���++
			node->hit_num++;
		}

		node->detach();
		node->attach(head);
	}

	return address;
}

/* 'address == -1' means remove the last one */
Node LRUCore::remove(unsigned address)
{
    Node *node;
	Node returnedNode;

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

	//Ҫ�������Ҫ���滻�Ŀ����ϸ��Ϣ
	returnedNode = (*node);

    delete node;
    return returnedNode;
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





// ָ����ӡsnapshoot���ļ���
void LRUCore::setOfs(ofstream* ofs_lru_core_snapshoot)
{
	this->ofs_lruCore_snapshoot = ofs_lru_core_snapshoot;
}
