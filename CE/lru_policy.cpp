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

void LRUPolicy::hit(unsigned address)
{
   
	unsigned length = lru_core.size();
	
	//1. ���������Ĳ��� (LRU����˳��)
	lru_core.update(address, unknown);
	//2���п��ܾͽ���Ԥȡ����
	this->doPrefetch(address);

	lru_core.printLRUCoreSnapshoot();
}

/*
	����δ���к���еĲ���
*/
void LRUPolicy::miss(unsigned address)
{
    unsigned length = lru_core.size();


	//1. ���������Ĳ���
	if (length == capacity)
	{
		 Node returnedNode= lru_core.remove(-1);

		 //��ӡ���滻��ȥ��page����Ϣ
		 if (returnedNode.pageState == prefetched)
		 {
			// *ofs_page_detail << old_address << ' ' << returnedNode.hit_num << endl;
		 }

		 lru_core.update(address,unprefetched);
	}
	else  // (length < this->capacity)
	{
		lru_core.update(address,unprefetched);
		//�����Ҫ++
		length++;
	}

	//2������Ԥȡ�Ĳ���
	this->doPrefetch(address);

	lru_core.printLRUCoreSnapshoot();
}

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


// ͳ���Ƿ�Ԥȡ��������Ŀ����Ϣ
void LRUPolicy::statistic()
{
	this->lru_core.statistic();
}




// ����һ��page�ǲ�����cache��
int LRUPolicy::findPage(unsigned int pageNo)
{
	return this->lru_core.findPage(pageNo);
}

void LRUPolicy::doPrefetch(int address)
{
	unsigned length = lru_core.size();
	RuleComponent* consequents=findAllConsequents(&(this->prefetchRules), "", "", address);
	
	if (consequents == NULL)
	{
		return;
	}

	RuleNode* temp=consequents->head;
	while (temp != NULL)
	{
		//���Ԥȡ�ĵ�ַ�Ѿ��ڻ����У�����������
		if (lru_core.exist(temp->offset))
		{
		
		}
		//Ԥȡ��ַ���ڻ����У��ұ���Ҫ�����滻
		else if (length == capacity)
		{
			Node returnedNode = lru_core.remove(-1);
		
			//��ӡ���滻��ȥ��page����Ϣ
			if (returnedNode.pageState == prefetched)
			{
				//*ofs_page_detail << old_old_address << ' ' << returnedNode.hit_num << endl;
			}
		
			lru_core.update(temp->offset, prefetched);
		
		
			this->prefetchReplaceCount++;
			this->prefetchCount++;
		}
		//Ԥȡ��ַ���ڻ����У��������
		else if (length<capacity)
		{
			lru_core.update(temp->offset, prefetched);
		
			this->prefetchCount++;
		
			//���ﳤ��������Ѿ��޸���
			length++;
		}
	}
}


// ����Ԥȡ����
//void LRUPolicy::doPrefetch(int address)
//{
//	unsigned length = lru_core.size();
//
//	vector<unsigned> prefetch_address = this->prefetchRules.findAllToAll(address, this->lru_core.getCacheShot(),this->capacity,1/16.0);
//
//	for (int i = 0; i < prefetch_address.size(); i++)
//	{
//		//���Ԥȡ�ĵ�ַ�Ѿ��ڻ����У�����������
//		if (lru_core.exist(prefetch_address[i]))
//		{
//
//		}
//		//Ԥȡ��ַ���ڻ����У��ұ���Ҫ�����滻
//		else if (length == capacity)
//		{
//			Node returnedNode = lru_core.remove(-1);
//
//			//��ӡ���滻��ȥ��page����Ϣ
//			if (returnedNode.pageState == prefetched)
//			{
//				//*ofs_page_detail << old_old_address << ' ' << returnedNode.hit_num << endl;
//			}
//
//			lru_core.update(prefetch_address[i], prefetched);
//
//
//			this->prefetchReplaceCount++;
//			this->prefetchCount++;
//		}
//		//Ԥȡ��ַ���ڻ����У��������
//		else if (length<capacity)
//		{
//			lru_core.update(prefetch_address[i], prefetched);
//
//			this->prefetchCount++;
//
//			//���ﳤ��������Ѿ��޸���
//			length++;
//		}
//	}
//
//	/*
//		vector<unsigned> prefetch_address = this->prefetchRules.find(address,this->lru_core.getCacheShot());
//
//	if (prefetch_address.size() != 0)
//	{
//		for (int i = 0; i < prefetch_address.size(); i++)
//		{	
//			
//
//			//���Ԥȡ�ĵ�ַ�Ѿ��ڻ����У�����������
//			if (lru_core.exist(prefetch_address[i]))
//			{
//			}
//			else //Ԥȡ���ڻ�����
//			{
//				length = lru_core.size();
//				//��������
//				if (length == capacity)
//				{
//					 Node returnedNode = lru_core.remove(-1);
//					 unsigned old_old_address = returnedNode.key;
//
//					 //��ӡ���滻��ȥ��page����Ϣ
//					 if (returnedNode.pageState == prefetched)
//					 {
//						 //*ofs_page_detail << old_old_address << ' ' << returnedNode.hit_num << endl;
//					 }
//					
//					lru_core.update(prefetch_address[i],prefetched);
//					this->prefetchReplaceCount++;
//					this->prefetchCount++;
//					//printAccessTrace(prefetch_address[i]);
//				}
//				//���滹δ��
//				else if (length < capacity)
//				{
//					lru_core.update(prefetch_address[i],prefetched);
//					
//					this->prefetchCount++;
//					//�������Ͼ�Ҫ�޸ģ�
//					length++;
//					//printAccessTrace(prefetch_address[i]);
//				}
//			}
//		
//		}
//	}
//	
//	
//	
//	*/
//
//}
