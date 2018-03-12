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
void PrefetchRules::put(vector<unsigned int> P, vector<unsigned int> Q, int support, double confidence)
{
	Rule rule(P, Q,support,confidence);
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
vector<unsigned int> PrefetchRules::findAllToAll(unsigned int P, unordered_map<unsigned, Node *>* cache,int capacity,double ratio)
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

	int willPrefetchNum = (capacity-cache->size())+capacity*ratio;
	int num = 0;

	sort(rules.begin(), rules.end(), Rule::decrease);
	
	vector<unsigned int > temp;
	temp.push_back(P);
	int index = this->findFrontPiece(temp);
	if (index != -1)
	{
		num += this->rules[index].Q.size();
		util.setAppend(myRules, this->rules[index].Q);
		willPrefetchNum -= this->rules[index].Q.size();
	}


	for (int i = 0; i < this->rules.size(); i++)
	{
		Rule rule = rules[i];
		if (util.isSubset(cacheShot, rule.P))
		{
			if (num >= willPrefetchNum)
			{
				break;
			}
			num += rule.Q.size();
			util.setAppend(myRules, rule.Q);
		}
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
//    1111,3333 ==> 2222,6666 #SUP: 492 #CONF : 0.82
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
		vector<string> rule_support_confidence;
		stringUtil.SplitString(s, rule_support_confidence, "#");
	
		// 1111 ==> 2222
		vector<string> P_Q;
		stringUtil.SplitString(rule_support_confidence[0], P_Q, "==>");

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

		//下面是拿支持度和置信度
		vector<string> name_value;
		stringUtil.SplitString(rule_support_confidence[1], name_value, ":");
		int support = stoi(name_value[1]);
		stringUtil.SplitString(rule_support_confidence[2], name_value, ":");
		double confidence = stod(name_value[1]);

		this->put(P, Q,support,confidence);
	}
}


// 找到P所在的下标，如果没有就返回-1
int PrefetchRules::findFrontPiece(vector<unsigned int> P)
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



