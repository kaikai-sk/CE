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

LRUPolicy::~LRUPolicy()
{
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
    lru_core.update(address);
	unsigned length = lru_core.size();
	
	vector<Action*> actionList;
	Action* action;
	
	//1. 进行正常的操作
	action = new Action;
	action->type = POLICY_IGNORE;
	actionList.push_back(action);
	//printAccessTrace(0);

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
					unsigned old_old_address = lru_core.remove(-1);
					lru_core.update(prefetch_address[i]);
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
					lru_core.update(prefetch_address[i]);
					action->prefetch_address = prefetch_address[i];
					action->type = POLICY_PREFETCH_APPEND;
					this->prefetchCount++;
					//printAccessTrace(prefetch_address[i]);
				}
			}
			actionList.push_back(action);
		}
	}

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
		old_address = lru_core.remove(-1);
		lru_core.update(address);

		action->type = POLICY_REPLACE;
		action->old_address = old_address;
		action->new_address = address;

		actionList.push_back(action);
	}
	else  // (length < this->capacity)
	{
		lru_core.update(address);

		action->type = POLICY_APPEND;
		action->new_address = address;
		actionList.push_back(action);
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
			//printAccessTrace(0);
		}
		//预取地址不在缓存中，且必须要进行替换
		else if (length == capacity)
		{
			unsigned old_old_address = lru_core.remove(-1);
			lru_core.update(prefetch_address[i]);
			action->prefetch_old_address = old_old_address;
			action->prefetch_address = prefetch_address[i];
			action->type = POLICY_PREFETCH_REPLACE;

			this->prefetchReplaceCount++;
			this->prefetchCount++;

			actionList.push_back(action);
			//printAccessTrace(prefetch_address[i]);
		}
		//预取地址不在缓存中，进行添加
		else if(length<capacity)
		{
			lru_core.update(prefetch_address[i]);
			action->prefetch_address = prefetch_address[i];
			action->type = POLICY_PREFETCH_APPEND;

			actionList.push_back(action);

			this->prefetchCount++;
		}

	}
	return actionList;

	////缓存已满
 //   if(length == capacity)
 //   {


	//	if (prefetch_address.size()!=0)
	//	{
	//		for (int i = 0; i < prefetch_address.size(); i++)
	//		{
	//			action = new Action;

	//			action->old_address = old_address;
	//			action->new_address = address;

	//			//如果预取的地址已经在缓存中，不做处理了
	//			if (lru_core.exist(prefetch_address[i]))
	//			{
	//				action->type = POLICY_REPLACE_IGNORE;
	//				//printAccessTrace(0);
	//			}
	//			else//如果预取的地址不在缓存中
	//			{
	//				unsigned old_old_address = lru_core.remove(-1);
	//				lru_core.update(prefetch_address[i]);
	//				action->prefetch_old_address = old_old_address;
	//				action->prefetch_address = prefetch_address[i];
	//				action->type = POLICY_REPLACE_REPLACE;

	//				this->prefetchReplaceCount++;
	//				this->prefetchCount++;

	//				//printAccessTrace(prefetch_address[i]);
	//			}
	//			actionList.push_back(action);
	//		}

	//	}
	//	else
	//	{
	//		action = new Action;
	//		action->old_address = old_address;
	//		action->new_address = address;

	//		action->type = POLICY_REPLACE;
	//		actionList.push_back(action);
	//		//printAccessTrace(0);
	//	}
 //       return actionList;
 //   }
	//// (length < this->capacity)
 //   else 
 //   {
 //       lru_core.update(address);
	//	action = new Action;
 //       action->new_address = address;

	//	if (prefetch_address.size()!=0)
	//	{
	//		for (int i = 0; i < prefetch_address.size(); i++)
	//		{
	//			action = new Action;
	//			//如果预取的地址已经在缓存中，不做处理了
	//			if (lru_core.exist(prefetch_address[i]))
	//			{
	//				action->type = POLICY_APPEND_IGNORE;

	//				//printAccessTrace(0);
	//				continue;
	//			}
	//			//接下来就是预取地址不在缓存中的情况
	//			length = lru_core.size();
	//			//缓存已满
	//			if (length == capacity)
	//			{
	//				unsigned old_old_address = lru_core.remove(-1);
	//				lru_core.update(prefetch_address[i]);
	//				action->prefetch_old_address = old_old_address;
	//				action->prefetch_address = prefetch_address[i];
	//				action->type = POLICY_APPEND_REPLACE;

	//				this->prefetchReplaceCount++;
	//				this->prefetchCount++;

	//				//printAccessTrace(prefetch_address[i]);
	//			}
	//			//缓存还未满的情况
	//			else if (length < capacity)
	//			{
	//				lru_core.update(prefetch_address[i]);
	//				action->prefetch_address = prefetch_address[i];
	//				action->type = POLICY_APPEND_APPEND;

	//				this->prefetchCount++;
	//				//printAccessTrace(prefetch_address[i]);
	//			}
	//			actionList.push_back(action);
	//		}
	//	}
	//	else
	//	{
	//		action->type = POLICY_APPEND;
	//		actionList.push_back(action);
	//		//printAccessTrace(0);
	//	}
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
	//(*ofs) << address << endl;
}


unsigned LRUPolicy::getPrefetchCount()
{
	return this->prefetchCount;
}

unsigned LRUPolicy::getPrefetchReplaceCount()
{
	return this->prefetchReplaceCount;
}
