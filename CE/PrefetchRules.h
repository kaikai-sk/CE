#pragma once
#include <map>
#include <vector>
using namespace std;


class PrefetchRules
{
private:
	multimap<unsigned, unsigned> rules;
public:
	PrefetchRules();
	~PrefetchRules();
	void put(unsigned int P, unsigned int Q);
	void pop(unsigned int P);
	vector<unsigned int> find(unsigned int P);
};

