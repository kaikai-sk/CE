#include "lru_core.h"

LRUCore::LRUCore()
    : head(new Node), tail(new Node)
{
    tail->attach(head);
	
	//初始化统计矩阵
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			this->staticMatrix[i][j] = 0;
		}
	}
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

/*
	打印cache的快照
*/
void LRUCore::printLRUCoreSnapshoot()
{
	for (auto &iter = records.begin(); iter != records.end(); iter++)
	{
		(*this->ofs_lruCore_snapshoot) << iter->first << ' ';
	}
	(*this->ofs_lruCore_snapshoot) << endl;
}

/*
	
*/
unsigned LRUCore::update(unsigned address,PageState pageState)
{
	Node *node;
	const auto &iter = records.find(address);

	/*这是插入的情况*/
	if (iter == records.end())
	{
		node = new Node;
		if (pageState == prefetched)
		{
			node->pageState = prefetched;
			this->prefetchedNodes.push_back(node);
		}
		else if(pageState==unprefetched)
		{
			node->pageState = unprefetched;
		}
		else
		{
			node->pageState = unknown;
		}

		node->key = address;
		node->attach(head);
		records[address] = node;
	}

	//这是命中的情况，要调整栈的顺序
	else
	{
		node = iter->second;

		node->isHit = true;

		//预取的块被命中的情况
		if (node->pageState == prefetched)
		{
			this->staticMatrix[0][0]++;
		}
		//命中的不是预取的情况
		else if (node->pageState == unprefetched)
		{
			this->staticMatrix[0][1]++;
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

	//要反回这个要被替换的块的详细信息
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





// 指定打印snapshoot的文件名
void LRUCore::setOfs(ofstream* ofs_lru_core_snapshoot)
{
	this->ofs_lruCore_snapshoot = ofs_lru_core_snapshoot;
}


// 统计预取情况和命中情况
void LRUCore::statistic()
{
	int hit_num = 0;
	for (int i = 0; i < prefetchedNodes.size(); i++)
	{
		if (prefetchedNodes[i]->isHit)
		{
			hit_num++;
		}
	}
	cout << "总共预取的节点的个数：" << this->prefetchedNodes.size() << " 被命中的个数是" << hit_num << endl;

	/*
	         是预读  不是预读
      命中
	  未命中
	*/
	cout << "        是预读  不是预读" << endl;
	cout << "命中    " << this->staticMatrix[0][0] << "      " << this->staticMatrix[0][1] << endl;
	cout << "未命中  " << this->staticMatrix[1][0] << "          " << this->staticMatrix[1][1] << endl;
}


// 拿到当前cache的快照
unordered_map<unsigned, Node *>* LRUCore::getCacheShot()
{
	return &(this->records);
}
