#include "PrefetchRules.h"

PrefetchRules::PrefetchRules()
{
}


PrefetchRules::~PrefetchRules()
{
}


void PrefetchRules::put(vector<unsigned int> P, vector<unsigned int> Q)
{
	Rule rule(P, Q);
	this->rules.push_back(rule);
}


void PrefetchRules::pop(unsigned int P)
{
	//删除所有键为P的键值对
	//this->rules.erase(P);
}

/*
	前件对应的所有后件都取出来了
*/
vector<unsigned int> PrefetchRules::find(unsigned int P, unordered_map<unsigned, Node *>* cache)
{
	SetUtil util;

	vector<unsigned int> myRules;
	vector<unsigned int> cacheShot;

	unordered_map<unsigned, Node*>::iterator iter = cache->begin();
	while (iter != cache->end())
	{
		cacheShot.push_back(iter->first);
		iter++;
	}

	for (int i = 0; i < this->rules.size(); i++)
	{
		Rule rule = rules[i];
		if (util.isSubset(cacheShot, rule.P))
		{
			util.setAppend(myRules, rule.Q);
		}
	}

	vector<unsigned int > temp;
	temp.push_back(P);
	int index=this->find(temp);
	if (index != -1)
	{
		util.setAppend(myRules, this->rules[index].Q);
	}

	return myRules;

	/*std::multimap<unsigned, unsigned>::iterator iter = rules.find(P);
	int i = 0;
	while (iter != rules.end())
	{
		if (iter->first == P)
		{
			myRules.push_back(iter->second);
		}
		iter++;
	}*/

}


// 查找到前件的所有后件
vector<unsigned int> PrefetchRules::findAll(unsigned int P)
{
	//CIT cit = dns.upper_bound("213.108.96.7");
	//// 输出: pythonzone.com，python-zone.com
	//while (cit != dns.end())
	//{
	//	cout << cit->second << endl;
	//	++cit;
	//}
	vector<unsigned int> temp;
	return temp;
}


// 加载规则文件
//文件格式：
//    1111 ==> 2222 #SUP: 492 #CONF : 0.82
void PrefetchRules::loadRules(string path)
{
	ifstream fin(path);
	string s;
	
	StringUtil stringUtil;

	while (getline(fin, s))
	{
		
		//将原始字符串分割为两部分
		// 1111 ==> 2222
		//SUP: 492 #CONF : 0.82
		vector<string> left_right;
		stringUtil.SplitString(s, left_right, "#");
	
		// 1111 ==> 2222
		vector<string> P_Q;
		stringUtil.SplitString(left_right[0], P_Q, "==>");

		vector<unsigned int> P , Q;
		vector<string> Ps,Qs;
		stringUtil.SplitString(P_Q[0], Ps, ",");
		stringUtil.SplitString(P_Q[1], Qs, ",");

		for (int i = 0; i < Ps.size(); i++)
		{
			P.push_back(stod(Ps[i]));
		}
		for (int i = 0; i < Qs.size(); i++)
		{
			Q.push_back(stod(Qs[i]));
		}
		this->put(P, Q);
	}
}


// 找到P所在的下标，如果没有就返回-1
int PrefetchRules::find(vector<unsigned int> P)
{
	SetUtil util;

	for (int i = 0; i < this->rules.size(); i++)
	{
		if (util.isEqual(P, rules[i].P))
		{
			return i;
			break;
		}
	}
	return -1;
}
