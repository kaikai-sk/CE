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
	
	//1. 进行正常的操作 (LRU调整顺序)
	lru_core.update(address, unknown);
	//2、有可能就进行预取操作
	this->doPrefetch(address);

	lru_core.printLRUCoreSnapshoot();
}

/*
	缓存未命中后进行的操作
*/
void LRUPolicy::miss(unsigned address)
{
    unsigned length = lru_core.size();


	//1. 进行正常的操作
	if (length == capacity)
	{
		 Node returnedNode= lru_core.remove(-1);

		 //打印被替换出去的page的信息
		 if (returnedNode.pageState == prefetched)
		 {
			// *ofs_page_detail << old_address << ' ' << returnedNode.hit_num << endl;
		 }

		 lru_core.update(address,unprefetched);
	}
	else  // (length < this->capacity)
	{
		lru_core.update(address,unprefetched);
		//这里就要++
		length++;
	}

	//2、进行预取的操作
	this->doPrefetch(address);

	lru_core.printLRUCoreSnapshoot();
}

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


// 统计是否预取与命中数目等信息
void LRUPolicy::statistic()
{
	this->lru_core.statistic();
}




// 查找一个page是不是在cache中
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
		//如果预取的地址已经在缓存中，不做处理了
		if (lru_core.exist(temp->offset))
		{
		
		}
		//预取地址不在缓存中，且必须要进行替换
		else if (length == capacity)
		{
			Node returnedNode = lru_core.remove(-1);
		
			//打印被替换出去的page的信息
			if (returnedNode.pageState == prefetched)
			{
				//*ofs_page_detail << old_old_address << ' ' << returnedNode.hit_num << endl;
			}
		
			lru_core.update(temp->offset, prefetched);
		
		
			this->prefetchReplaceCount++;
			this->prefetchCount++;
		}
		//预取地址不在缓存中，进行添加
		else if (length<capacity)
		{
			lru_core.update(temp->offset, prefetched);
		
			this->prefetchCount++;
		
			//这里长度这里就已经修改了
			length++;
		}
	}
}


// 进行预取操作
//void LRUPolicy::doPrefetch(int address)
//{
//	unsigned length = lru_core.size();
//
//	vector<unsigned> prefetch_address = this->prefetchRules.findAllToAll(address, this->lru_core.getCacheShot(),this->capacity,1/16.0);
//
//	for (int i = 0; i < prefetch_address.size(); i++)
//	{
//		//如果预取的地址已经在缓存中，不做处理了
//		if (lru_core.exist(prefetch_address[i]))
//		{
//
//		}
//		//预取地址不在缓存中，且必须要进行替换
//		else if (length == capacity)
//		{
//			Node returnedNode = lru_core.remove(-1);
//
//			//打印被替换出去的page的信息
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
//		//预取地址不在缓存中，进行添加
//		else if (length<capacity)
//		{
//			lru_core.update(prefetch_address[i], prefetched);
//
//			this->prefetchCount++;
//
//			//这里长度这里就已经修改了
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
//			//如果预取的地址已经在缓存中，不做处理了
//			if (lru_core.exist(prefetch_address[i]))
//			{
//			}
//			else //预取不在缓存中
//			{
//				length = lru_core.size();
//				//缓存已满
//				if (length == capacity)
//				{
//					 Node returnedNode = lru_core.remove(-1);
//					 unsigned old_old_address = returnedNode.key;
//
//					 //打印被替换出去的page的信息
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
//				//缓存还未满
//				else if (length < capacity)
//				{
//					lru_core.update(prefetch_address[i],prefetched);
//					
//					this->prefetchCount++;
//					//这里马上就要修改；
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
