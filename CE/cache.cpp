#include "cache.h"
#include "policy.h"


/*
	���Ƿ�cache����
*/
int Cache::query(unsigned address)
{
	vector<int> accessPageNo;

	//��ַ����cache��
    if(this->findPage(address)==-1)
	{
       policy->miss(address);
        return CACHE_MISS;
    }
	//���������е����
    else
    {
        policy->hit(address);
        return CACHE_HIT;
    }
}

// ����һ��page�ǲ�����cache����
int Cache::findPage(unsigned int address)
{
	return ((LRUPolicy*)(this->policy))->findPage(address);
}

// ��ӡtrace,������ӡ���
void Cache::printAccessTrace(vector<int> pageNos)
{
	for (int i = 0; i < pageNos.size(); i++)
	{
		(*ofs) << pageNos[i] << " ";
	}
	(*ofs) << endl;
}

// ���page no���ļ��У�������¼trace
void Cache::printAccessTrace(int pageNo)
{
	(*ofs) << pageNo << endl;
}

unsigned Cache::getReplaceNum()
{
	return this->replaceNum;
}