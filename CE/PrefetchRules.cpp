#include "PrefetchRules.h"



PrefetchRules::PrefetchRules()
{
}


PrefetchRules::~PrefetchRules()
{
}


void PrefetchRules::put(unsigned int P, unsigned int Q)
{
	this->rules.insert(pair<unsigned, unsigned>(P,Q));
}


void PrefetchRules::pop(unsigned int P)
{
	//删除所有键为P的键值对
	this->rules.erase(P);
}


vector<unsigned int> PrefetchRules::find(unsigned int P)
{
	//unsigned int uCount = rules.count(P);
	vector<unsigned int> myRules;
	std::multimap<unsigned, unsigned>::iterator iter = rules.find(P);
	int i = 0;
	while (iter != rules.end())
	{
		if (iter->first == P)
		{
			myRules.push_back(iter->second);
		}
		iter++;
	}
	return myRules;
}
