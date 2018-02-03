#include "lru_policy.h"


LRUPolicy::LRUPolicy(unsigned capacity)
    : capacity(capacity)
{
	this->prefetchCount = 0;
	this->prefetchReplaceCount = 0;
}

LRUPolicy::LRUPolicy(unsigned capacity, PrefetchRules prefetchRules)
{
	this->capacity = capacity;
	this->prefetchRules = prefetchRules;
	this->lru_core.setCapacity(capacity);
	this->prefetchCount = 0;
	this->prefetchReplaceCount = 0;
}

LRUPolicy::LRUPolicy(unsigned capacity, PrefetchRules prefetchRules,ofstream* ofs)
{
	this->capacity = capacity;
	this->prefetchRules = prefetchRules;
	this->lru_core.setCapacity(capacity);
	this->ofs = ofs;
	this->prefetchCount = 0;
	this->prefetchReplaceCount = 0;
}

LRUPolicy::LRUPolicy(unsigned capacity, PrefetchRules prefetchRules, ofstream* ofs,ofstream* ofs_lru_core_snapshoot)
{
	this->capacity = capacity;
	this->prefetchRules = prefetchRules;
	this->lru_core.setCapacity(capacity);
	this->ofs = ofs;
	this->prefetchCount = 0;
	this->prefetchReplaceCount = 0;
	this->ofs_lru_core_snapshoot = ofs_lru_core_snapshoot;
	this->lru_core.setOfs(ofs_lru_core_snapshoot);
}

LRUPolicy::LRUPolicy(unsigned capacity, PrefetchRules prefetchRules, ofstream* ofs, ofstream* ofs_lru_core_snapshoot, ofstream* ofs_page_detail)
{
	this->capacity = capacity;
	this->prefetchRules = prefetchRules;
	this->lru_core.setCapacity(capacity);
	this->ofs = ofs;
	this->prefetchCount = 0;
	this->prefetchReplaceCount = 0;
	this->ofs_lru_core_snapshoot = ofs_lru_core_snapshoot;
	this->lru_core.setOfs(ofs_lru_core_snapshoot);
	this->ofs_page_detail = ofs_page_detail;
}

LRUPolicy::~LRUPolicy()
{
	delete ofs;
}

//Action &LRUPolicy::hit(unsigned address)
//{
//	lru_core.update(address);
//
//	action.type = POLICY_IGNORE;
//	return action;
//}

vector<Action*> LRUPolicy::hit(unsigned address)
{
    lru_core.update(address,0);
	unsigned length = lru_core.size();
	
	vector<Action*> actionList;
	Action* action;
	
	//1. 进行正常的操作
	action = new Action;
	action->type = POLICY_IGNORE;
	actionList.push_back(action);

	//2、有可能就进行预取操作

	vector<unsigned> prefetch_address = this->prefetchRules.find(address);

	if (prefetch_address.size() != 0)
	{
		for (int i = 0; i < prefetch_address.size(); i++)
		{	
			action = new Action;

			//如果预取的地址已经在缓存中，不做处理了
			if (lru_core.exist(prefetch_address[i]))
			{
				action->type = POLICY_IGNORE;
				//printAccessTrace(0);
			}
			else //预取不在缓存中
			{
				length = lru_core.size();
				//缓存已满
				if (length == capacity)
				{
					 Node returnedNode = lru_core.remove(-1);
					 unsigned old_old_address = returnedNode.key;

					 //打印被替换出去的page的信息
					 if (returnedNode.tag == 1)
					 {
						 *ofs_page_detail << old_old_address << ' ' << returnedNode.hit_num << endl;
					 }
					
					lru_core.update(prefetch_address[i],1);
					action->prefetch_old_address = old_old_address;
					action->prefetch_address = prefetch_address[i];
					action->type = POLICY_PREFETCH_REPLACE;
					this->prefetchReplaceCount++;
					this->prefetchCount++;
					//printAccessTrace(prefetch_address[i]);
				}
				//缓存还未满
				else if (length < capacity)
				{
					lru_core.update(prefetch_address[i],1);
					action->prefetch_address = prefetch_address[i];
					action->type = POLICY_PREFETCH_APPEND;
					
					this->prefetchCount++;
					//这里马上就要修改；
					length++;
					//printAccessTrace(prefetch_address[i]);
				}
			}
			actionList.push_back(action);
		}
	}
	lru_core.printLRUCoreSnapshoot();
    return actionList;
}

/*
	缓存未命中后进行的操作
*/
vector<Action*>LRUPolicy::miss(unsigned address)
{
    unsigned old_address,old_address1;
    unsigned length = lru_core.size();

	Action* action;
	vector<Action*> actionList;


	//1. 进行正常的操作
	action = new Action;
	if (length == capacity)
	{
		 Node returnedNode= lru_core.remove(-1);
		 old_address = returnedNode.key;

		 //打印被替换出去的page的信息
		 if (returnedNode.tag == 1)
		 {
			 *ofs_page_detail << old_address << ' ' << returnedNode.hit_num << endl;
		 }

		 lru_core.update(address,0);

		action->type = POLICY_REPLACE;
		action->old_address = old_address;
		action->new_address = address;

		actionList.push_back(action);
	}
	else  // (length < this->capacity)
	{
		lru_core.update(address,0);

		action->type = POLICY_APPEND;
		action->new_address = address;
		actionList.push_back(action);

		//这里就要++
		length++;
	}

	//2、进行预取的操作
	vector<unsigned> prefetch_address=this->prefetchRules.find(address);
	
	for (int i = 0; i < prefetch_address.size(); i++)
	{
		action = new Action;

		//如果预取的地址已经在缓存中，不做处理了
		if (lru_core.exist(prefetch_address[i]))
		{
			action->type = POLICY_IGNORE;
			actionList.push_back(action);
		}
		//预取地址不在缓存中，且必须要进行替换
		else if (length == capacity)
		{
			Node returnedNode= lru_core.remove(-1);
			unsigned old_old_address = returnedNode.key;

			//打印被替换出去的page的信息
			if (returnedNode.tag == 1)
			{
				*ofs_page_detail << old_old_address << ' ' << returnedNode.hit_num << endl;
			}

			lru_core.update(prefetch_address[i],1);
			action->prefetch_old_address = old_old_address;
			action->prefetch_address = prefetch_address[i];
			action->type = POLICY_PREFETCH_REPLACE;

			this->prefetchReplaceCount++;
			this->prefetchCount++;

			actionList.push_back(action);
		}
		//预取地址不在缓存中，进行添加
		else if(length<capacity)
		{
			lru_core.update(prefetch_address[i],1);
			action->prefetch_address = prefetch_address[i];
			action->type = POLICY_PREFETCH_APPEND;

			actionList.push_back(action);

			this->prefetchCount++;

			//这里长度这里就已经修改了
			length++;
		}

	}
	lru_core.printLRUCoreSnapshoot();
	return actionList;
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


// 打印出预取的trace
void LRUPolicy::printAccessTrace(unsigned address)
{
	(*ofs) << address << endl;
}


unsigned LRUPolicy::getPrefetchCount()
{
	return this->prefetchCount;
}

unsigned LRUPolicy::getPrefetchReplaceCount()
{
	return this->prefetchReplaceCount;
}
