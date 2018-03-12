#include "cache.h"
#include "policy.h"


/*
	看是否cache命中
*/
int Cache::query(unsigned address)
{
	vector<int> accessPageNo;

	//地址不在cache中
    if(this->findPage(address)==-1)
	{
       policy->miss(address);
        return CACHE_MISS;
    }
	//这里是命中的情况
    else
    {
        policy->hit(address);
        return CACHE_HIT;
    }
}

// 查找一个page是不是在cache当中
int Cache::findPage(unsigned int address)
{
	return ((LRUPolicy*)(this->policy))->findPage(address);
}

// 打印trace,连续打印多个
void Cache::printAccessTrace(vector<int> pageNos)
{
	for (int i = 0; i < pageNos.size(); i++)
	{
		(*ofs) << pageNos[i] << " ";
	}
	(*ofs) << endl;
}

// 输出page no到文件中，即，记录trace
void Cache::printAccessTrace(int pageNo)
{
	(*ofs) << pageNo << endl;
}

unsigned Cache::getReplaceNum()
{
	return this->replaceNum;
}