#include "lru_policy.h"


LRUPolicy::LRUPolicy(unsigned capacity)
    : capacity(capacity)
{
}

LRUPolicy::LRUPolicy(unsigned capacity, PrefetchRules prefetchRules)
{
	this->capacity = capacity;
	this->prefetchRules = prefetchRules;
	this->lru_core.setCapacity(capacity);
}

LRUPolicy::LRUPolicy(unsigned capacity, PrefetchRules prefetchRules,ofstream* ofs)
{
	this->capacity = capacity;
	this->prefetchRules = prefetchRules;
	this->lru_core.setCapacity(capacity);
	this->ofs = ofs;
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

Action &LRUPolicy::hit(unsigned address)
{
    lru_core.update(address);
	unsigned length = lru_core.size();
	vector<unsigned> prefetch_address = this->prefetchRules.find(address);
	if (prefetch_address.size() != 0)
	{
		for (int i = 0; i < prefetch_address.size(); i++)
		{
			//如果预取的地址已经在缓存中，不做处理了
			if (lru_core.exist(prefetch_address[i]))
			{
				action.type = POLICY_IGNORE_IGNORE;
				printAccessTrace(0);
			}
			else //预取不在缓存中
			{
				length = lru_core.size();
				//缓存已满
				if (length == capacity)
				{
					unsigned old_old_address = lru_core.remove(-1);
					lru_core.update(prefetch_address[i]);
					action.prefetch_old_address = old_old_address;
					action.prefetch_address = prefetch_address[i];
					action.type = POLICY_IGNORE_REPLACE;
					printAccessTrace(prefetch_address[i]);
				}
			}
		}




		//if (length == capacity)
		//{
		//	for (int i = 0; i < prefetch_address.size(); i++)
		//	{
		//		//如果预取的地址已经在缓存中，不做处理了
		//		if (lru_core.exist(prefetch_address[i]))
		//		{
		//			action.type = POLICY_IGNORE_IGNORE;
		//			printAccessTrace(0);
		//		}
		//		else//如果预取的地址不在缓存中
		//		{
		//			unsigned old_old_address = lru_core.remove(-1);
		//			lru_core.update(prefetch_address[i]);
		//			action.prefetch_old_address = old_old_address;
		//			action.prefetch_address = prefetch_address[i];
		//			action.type = POLICY_IGNORE_REPLACE;
		//			printAccessTrace(prefetch_address[i]);
		//		}
		//	}
		//}
		//else
		//{
		//	for (int i = 0; i < prefetch_address.size(); i++)
		//	{
		//		//如果预取的地址已经在缓存中，不做处理了
		//		if (lru_core.exist(prefetch_address[i]))
		//		{
		//			action.type = POLICY_IGNORE_IGNORE;
		//			printAccessTrace(0);
		//		}
		//		else//如果预取的地址不在缓存中
		//		{
		//			lru_core.update(prefetch_address[i]);
		//			action.prefetch_address = prefetch_address[i];
		//			action.type = POLICY_IGNORE_APPEND;
		//			printAccessTrace(prefetch_address[i]);
		//		}
		//	}
		//}
	}
	else
	{
		action.type = POLICY_IGNORE;
		printAccessTrace(0);
	}
    return action;
}

/*
	缓存未命中后进行的操作
*/
Action &LRUPolicy::miss(unsigned address)
{
    unsigned old_address,old_address1;
    unsigned length = lru_core.size();
	vector<unsigned> prefetch_address=this->prefetchRules.find(address);

    if(length == capacity)
    {
        old_address = lru_core.remove(-1);
        lru_core.update(address);

		action.old_address = old_address;
		action.new_address = address;

		if (prefetch_address.size()!=0)
		{
			for (int i = 0; i < prefetch_address.size(); i++)
			{
				//如果预取的地址已经在缓存中，不做处理了
				if (lru_core.exist(prefetch_address[i]))
				{
					action.type = POLICY_REPLACE_IGNORE;
					printAccessTrace(0);
				}
				else//如果预取的地址不在缓存中
				{
					unsigned old_old_address = lru_core.remove(-1);
					lru_core.update(prefetch_address[i]);
					action.prefetch_old_address = old_old_address;
					action.prefetch_address = prefetch_address[i];
					action.type = POLICY_REPLACE_REPLACE;
					printAccessTrace(prefetch_address[i]);
				}
			}

		}
		else
		{
			action.type = POLICY_REPLACE;
			printAccessTrace(0);
		}
        return action;
    }
    else  // (length < this->capacity)
    {
        lru_core.update(address);
        action.new_address = address;

		if (prefetch_address.size()!=0)
		{
			unsigned appended_length = lru_core.size();
			//如果添加了一个之后满了
			if (appended_length == capacity)
			{
				for (int i = 0; i < prefetch_address.size(); i++)
				{
					//如果预取的地址已经在缓存中，不做处理了
					if (lru_core.exist(prefetch_address[i]))
					{
						action.type = POLICY_APPEND_IGNORE;
						printAccessTrace(0);
					}
					else//如果预取的地址不在缓存中
					{
						unsigned old_old_address = lru_core.remove(-1);
						lru_core.update(prefetch_address[i]);
						action.prefetch_old_address = old_old_address;
						action.prefetch_address = prefetch_address[i];
						action.type = POLICY_APPEND_REPLACE;
						printAccessTrace(prefetch_address[i]);
					}
				}
			}
			else
			{
				for (int i = 0; i < prefetch_address.size(); i++)
				{
					//如果预取的地址已经在缓存中，不做处理了
					if (lru_core.exist(prefetch_address[i]))
					{
						action.type = POLICY_APPEND_IGNORE;
						printAccessTrace(0);
					}
					else//如果预取的地址不在缓存中
					{
						lru_core.update(prefetch_address[i]);
						action.prefetch_address = prefetch_address[i];
						action.type = POLICY_APPEND_APPEND;
						printAccessTrace(prefetch_address[i]);
					}
				}

			}
		}
		else
		{
			action.type = POLICY_APPEND;
			printAccessTrace(0);
		}
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


// 打印出预取的trace
void LRUPolicy::printAccessTrace(unsigned address)
{
	(*ofs) << address << endl;
}
