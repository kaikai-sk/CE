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
	
	//1. ���������Ĳ���
	action = new Action;
	action->type = POLICY_IGNORE;
	actionList.push_back(action);

	//2���п��ܾͽ���Ԥȡ����

	vector<unsigned> prefetch_address = this->prefetchRules.find(address);

	if (prefetch_address.size() != 0)
	{
		for (int i = 0; i < prefetch_address.size(); i++)
		{	
			action = new Action;

			//���Ԥȡ�ĵ�ַ�Ѿ��ڻ����У�����������
			if (lru_core.exist(prefetch_address[i]))
			{
				action->type = POLICY_IGNORE;
				//printAccessTrace(0);
			}
			else //Ԥȡ���ڻ�����
			{
				length = lru_core.size();
				//��������
				if (length == capacity)
				{
					 Node returnedNode = lru_core.remove(-1);
					 unsigned old_old_address = returnedNode.key;

					 //��ӡ���滻��ȥ��page����Ϣ
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
				//���滹δ��
				else if (length < capacity)
				{
					lru_core.update(prefetch_address[i],1);
					action->prefetch_address = prefetch_address[i];
					action->type = POLICY_PREFETCH_APPEND;
					
					this->prefetchCount++;
					//�������Ͼ�Ҫ�޸ģ�
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
	����δ���к���еĲ���
*/
vector<Action*>LRUPolicy::miss(unsigned address)
{
    unsigned old_address,old_address1;
    unsigned length = lru_core.size();

	Action* action;
	vector<Action*> actionList;


	//1. ���������Ĳ���
	action = new Action;
	if (length == capacity)
	{
		 Node returnedNode= lru_core.remove(-1);
		 old_address = returnedNode.key;

		 //��ӡ���滻��ȥ��page����Ϣ
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

		//�����Ҫ++
		length++;
	}

	//2������Ԥȡ�Ĳ���
	vector<unsigned> prefetch_address=this->prefetchRules.find(address);
	
	for (int i = 0; i < prefetch_address.size(); i++)
	{
		action = new Action;

		//���Ԥȡ�ĵ�ַ�Ѿ��ڻ����У�����������
		if (lru_core.exist(prefetch_address[i]))
		{
			action->type = POLICY_IGNORE;
			actionList.push_back(action);
		}
		//Ԥȡ��ַ���ڻ����У��ұ���Ҫ�����滻
		else if (length == capacity)
		{
			Node returnedNode= lru_core.remove(-1);
			unsigned old_old_address = returnedNode.key;

			//��ӡ���滻��ȥ��page����Ϣ
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
		//Ԥȡ��ַ���ڻ����У��������
		else if(length<capacity)
		{
			lru_core.update(prefetch_address[i],1);
			action->prefetch_address = prefetch_address[i];
			action->type = POLICY_PREFETCH_APPEND;

			actionList.push_back(action);

			this->prefetchCount++;

			//���ﳤ��������Ѿ��޸���
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


// ��ӡ��Ԥȡ��trace
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
