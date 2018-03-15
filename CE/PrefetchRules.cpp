#include "PrefetchRules.h"

void printNode(RuleNode* node)
{
	printf("%s %s %d", node->process_name, node->file_name, node->offset);
}

void printComponent(RuleComponent* P)
{
	RuleNode* current=P->head;

	while (current != NULL)
	{
		printNode(current);
		if (current->next != NULL)
		{
			printf(",");
		}
		current = current->next;
	}

}

//打印一条规则
void printARule(Rule* rule)
{
	RuleComponent* P = &(rule->P);
	RuleComponent* Q = &(rule->Q);

	printComponent(P);
	printf("==>");
	printComponent(Q);
	printf("#sup=%f", rule->support);
	printf("#conf=%f", rule->confidence);
	printf("\n");
}

//找出一个前件对应的所有后件
RuleComponent* findAllConsequents(PrefetchRules* prefetchRules,char* process_name, char* file_name, int offset)
{
	Rule* rule;
	rule = prefetchRules->head;
	RuleComponent* temp=(RuleComponent*)malloc(sizeof(RuleComponent));
	
	int size = 0;

	while (rule != NULL)
	{
		RuleComponent* P = &(rule->P);
		if (sizeOfComponent(P) == 1 && P->head->offset==offset)
		{
			RuleNode* tempNode = rule->Q.head;
			while (tempNode != NULL)
			{
				addNode(temp, tempNode);
				size++;
			}
		}
		rule = rule->next;

	}

	if (size == 0)
	{
		return NULL;
	}
	else
	{
		return temp;
	}
}

/*
	前件对应的所有后件都取出来了
*/
//vector<unsigned int> findAllToAll(unsigned int P, unordered_map<unsigned, Node *>* cache,int capacity,double ratio)
//{
//	SetUtil util;
//
//	vector<unsigned int> myRules;
//	vector<unsigned int> cacheShot;
//
//	unordered_map<unsigned, Node*>::iterator iter = cache->begin();
//	while (iter != cache->end())
//	{
//		cacheShot.push_back(iter->first);
//		iter++;
//	}
//
//	int willPrefetchNum = (capacity-cache->size())+capacity*ratio;
//	int num = 0;
//
//	sort(rules.begin(), rules.end(), Rule::decrease);
//	
//	vector<unsigned int > temp;
//	temp.push_back(P);
//	int index = this->findFrontPiece(temp);
//	if (index != -1)
//	{
//		num += this->rules[index].Q.size();
//		util.setAppend(myRules, this->rules[index].Q);
//		willPrefetchNum -= this->rules[index].Q.size();
//	}
//
//
//	for (int i = 0; i < this->rules.size(); i++)
//	{
//		Rule rule = rules[i];
//		if (util.isSubset(cacheShot, rule.P))
//		{
//			if (num >= willPrefetchNum)
//			{
//				break;
//			}
//			num += rule.Q.size();
//			util.setAppend(myRules, rule.Q);
//		}
//	}
//
//	
//
//	return myRules;
//
//	/*std::multimap<unsigned, unsigned>::iterator iter = rules.find(P);
//	int i = 0;
//	while (iter != rules.end())
//	{
//		if (iter->first == P)
//		{
//			myRules.push_back(iter->second);
//		}
//		iter++;
//	}*/
//
//}

//打印预取规则
void printRules(PrefetchRules *prefetchRules)
{
	Rule* temp;
	temp = prefetchRules->head;

	while (temp != NULL)
	{
		printARule(temp);
		printf("\n");
		temp = temp->next;
	}
}

//读取文件到内存中
//文件格式：
//    weibo temp 111,weixin ttt 222 ==> tencent yyy 678 #sup = 0.5#conf = 0.6
void loadRules(PrefetchRules *prefetchRules,string path)
{
	ifstream fin(path);
	string s;

	StringUtil stringUtil;

	while (getline(fin, s))
	{
		//将原始字符串分割为三部分
		// 1111 ==> 2222
		// #SUP: 492 
		// #CONF : 0.82
		vector<string> rule_support_confidence;
		stringUtil.SplitString(s, rule_support_confidence, "#");

		//  weibo temp 111,weixin ttt 222 
		//  tencent yyy 678
		vector<string> P_Q;
		stringUtil.SplitString(rule_support_confidence[0], P_Q, "==>");

		//weibo temp 111
		//weixin ttt 222
		vector<string> Ps, Qs;
		stringUtil.SplitString(P_Q[0], Ps, ",");
		stringUtil.SplitString(P_Q[1], Qs, ",");

		RuleComponent P;
		P.head = NULL;

		for (int i = 0; i < Ps.size(); i++)
		{
			vector<string> tempNode;
			stringUtil.SplitString(Ps[i], tempNode, " ");
			char* process_name=NULL;
			char* file_name=NULL;
			process_name=const_cast<char*>(tempNode[0].c_str());
			file_name= const_cast<char*>(tempNode[1].c_str());
			addNode(&P, process_name, file_name, stoi(tempNode[2]));
		}

		RuleComponent Q;
		Q.head = NULL;

		for (int i = 0; i < Qs.size(); i++)
		{
			vector<string> tempNode;
			stringUtil.SplitString(Qs[i], tempNode, " ");
			char* process_name=NULL;
			char* file_name=NULL;
			process_name = const_cast<char*>(tempNode[0].c_str());
			file_name = const_cast<char*>(tempNode[1].c_str());
			addNode(&Q, process_name, file_name, stoi(tempNode[2]));
		}

		//下面是拿支持度和置信度
		vector<string> name_value;
		stringUtil.SplitString(rule_support_confidence[1], name_value, "=");
		int support = stoi(name_value[1]);
		stringUtil.SplitString(rule_support_confidence[2], name_value, "=");
		double confidence = stod(name_value[1]);

		put(prefetchRules, P, Q, support, confidence);
	}
}

bool nodeEquals(RuleNode* a, RuleNode* b)
{
	if (a->process_name == b->process_name && a->file_name == b->file_name && a->offset == b->offset)
	{
		return true;
	}
	return false;
}

int sizeOfComponent(RuleComponent* ruleComponent)
{
	RuleNode* node = ruleComponent->head;
	int size = 0;
	while (node != NULL)
	{
		size++;
		node = node->next;
	}
	return size;
}

//为一个规则前件或者后件添加Node
void addNode(RuleComponent* component, RuleNode* ruleNode)
{
	RuleNode* temp;

	/* 判断链表是否为空 */
	if (NULL == component->head)
	{
		/* 为空 */
		component->head = ruleNode;
		component->head->next = NULL;
	}
	else
	{
		/* 不为空 */
		temp = component->head;
		while (temp)
		{
			if (NULL == temp->next)
			{
				temp->next = ruleNode;
				ruleNode->next = NULL;
			}
			temp = temp->next;
		}
	}
}

//为一个规则前件或者后件添加Node
void addNode(RuleComponent* component, char* process_name, char* file_name, int offset)
{	
	RuleNode* node=(RuleNode*)malloc(sizeof(RuleNode));
	strcpy(node->process_name,process_name);
	strcpy(node->file_name,file_name);
	node->offset = offset;
	node->next = NULL;

	addNode(component, node);
}

//添加一项规则
void put(PrefetchRules* prefetchRules, Rule* rule)
{
	Rule* temp;

	/* 判断链表是否为空 */
	if (NULL == prefetchRules->head)
	{
		/* 为空 */
		prefetchRules->head = rule;
		prefetchRules->head->next = NULL;
	}
	else
	{
		/* 不为空 */
		temp = prefetchRules->head;
		while (temp)
		{
			if (NULL == temp->next)
			{
				temp->next = rule;
				rule->next = NULL;
			}
			temp = temp->next;
		}
	}
}


void put(PrefetchRules* prefetchRules, RuleComponent P, RuleComponent Q)
{	
	Rule* rule=(Rule*)malloc(sizeof(Rule));
	rule->P = P;
	rule->Q = Q;
	rule->next = NULL;

	put(prefetchRules, rule);
}

void put(PrefetchRules* prefetchRules, RuleComponent P, RuleComponent Q, int support, double confidence)
{
	Rule* rule=(Rule*)malloc(sizeof(Rule));
	rule->P = P;
	rule->Q = Q;
	rule->support = support;
	rule->confidence = confidence;
	rule->next = NULL;

	put(prefetchRules, rule);
}











//
//PrefetchRules::PrefetchRules()
//{
//}
//
//
//PrefetchRules::~PrefetchRules()
//{
//}


//void PrefetchRules::put(vector<unsigned int> P, vector<unsigned int> Q)
//{
//	Rule rule(P, Q);
//	this->rules.push_back(rule);
//}
//void PrefetchRules::put(vector<unsigned int> P, vector<unsigned int> Q, int support, double confidence)
//{
//	Rule rule(P, Q,support,confidence);
//	this->rules.push_back(rule);
//}





//void PrefetchRules::pop(unsigned int P)
//{
//	//删除所有键为P的键值对
//	//this->rules.erase(P);
//}

/*
	前件对应的所有后件都取出来了
*/
//vector<unsigned int> PrefetchRules::findAllToAll(unsigned int P, unordered_map<unsigned, Node *>* cache,int capacity,double ratio)
//{
//	SetUtil util;
//
//	vector<unsigned int> myRules;
//	vector<unsigned int> cacheShot;
//
//	unordered_map<unsigned, Node*>::iterator iter = cache->begin();
//	while (iter != cache->end())
//	{
//		cacheShot.push_back(iter->first);
//		iter++;
//	}
//
//	int willPrefetchNum = (capacity-cache->size())+capacity*ratio;
//	int num = 0;
//
//	sort(rules.begin(), rules.end(), Rule::decrease);
//	
//	vector<unsigned int > temp;
//	temp.push_back(P);
//	int index = this->findFrontPiece(temp);
//	if (index != -1)
//	{
//		num += this->rules[index].Q.size();
//		util.setAppend(myRules, this->rules[index].Q);
//		willPrefetchNum -= this->rules[index].Q.size();
//	}
//
//
//	for (int i = 0; i < this->rules.size(); i++)
//	{
//		Rule rule = rules[i];
//		if (util.isSubset(cacheShot, rule.P))
//		{
//			if (num >= willPrefetchNum)
//			{
//				break;
//			}
//			num += rule.Q.size();
//			util.setAppend(myRules, rule.Q);
//		}
//	}
//
//	
//
//	return myRules;
//
//	/*std::multimap<unsigned, unsigned>::iterator iter = rules.find(P);
//	int i = 0;
//	while (iter != rules.end())
//	{
//		if (iter->first == P)
//		{
//			myRules.push_back(iter->second);
//		}
//		iter++;
//	}*/
//
//}


// 查找到前件的所有后件
//vector<unsigned int> PrefetchRules::findAll(unsigned int P)
//{
//	//CIT cit = dns.upper_bound("213.108.96.7");
//	//// 输出: pythonzone.com，python-zone.com
//	//while (cit != dns.end())
//	//{
//	//	cout << cit->second << endl;
//	//	++cit;
//	//}
//	vector<unsigned int> temp;
//	return temp;
//}


//void PrefetchRules::loadRules(string path)
//{
//	ifstream fin(path);
//	string s;
//	
//	StringUtil stringUtil;
//
//	while (getline(fin, s))
//	{
//		//将原始字符串分割为两部分
//		// 1111 ==> 2222
//		//SUP: 492 #CONF : 0.82
//		vector<string> rule_support_confidence;
//		stringUtil.SplitString(s, rule_support_confidence, "#");
//
//		// 1111 ==> 2222
//		vector<string> P_Q;
//		stringUtil.SplitString(rule_support_confidence[0], P_Q, "==>");
//
//		vector<unsigned int> P , Q;
//		vector<string> Ps,Qs;
//		stringUtil.SplitString(P_Q[0], Ps, ",");
//		stringUtil.SplitString(P_Q[1], Qs, ",");
//
//		for (int i = 0; i < Ps.size(); i++)
//		{
//			P.push_back(stod(Ps[i]));
//		}
//		for (int i = 0; i < Qs.size(); i++)
//		{
//			Q.push_back(stod(Qs[i]));
//		}
//
//		//下面是拿支持度和置信度
//		vector<string> name_value;
//		stringUtil.SplitString(rule_support_confidence[1], name_value, ":");
//		int support = stoi(name_value[1]);
//		stringUtil.SplitString(rule_support_confidence[2], name_value, ":");
//		double confidence = stod(name_value[1]);
//
//		this->put(P, Q,support,confidence);
//	}
//}




// 加载规则文件
//文件格式：
//    1111,3333 ==> 2222,6666 #SUP: 492 #CONF : 0.82
//void PrefetchRules::loadRules(string path)
//{
//	ifstream fin(path);
//	string s;
//	
//	StringUtil stringUtil;
//
//	while (getline(fin, s))
//	{
//		//将原始字符串分割为两部分
//		// 1111 ==> 2222
//		//SUP: 492 #CONF : 0.82
//		vector<string> rule_support_confidence;
//		stringUtil.SplitString(s, rule_support_confidence, "#");
//
//		// 1111 ==> 2222
//		vector<string> P_Q;
//		stringUtil.SplitString(rule_support_confidence[0], P_Q, "==>");
//
//		vector<unsigned int> P , Q;
//		vector<string> Ps,Qs;
//		stringUtil.SplitString(P_Q[0], Ps, ",");
//		stringUtil.SplitString(P_Q[1], Qs, ",");
//
//		for (int i = 0; i < Ps.size(); i++)
//		{
//			P.push_back(stod(Ps[i]));
//		}
//		for (int i = 0; i < Qs.size(); i++)
//		{
//			Q.push_back(stod(Qs[i]));
//		}
//
//		//下面是拿支持度和置信度
//		vector<string> name_value;
//		stringUtil.SplitString(rule_support_confidence[1], name_value, ":");
//		int support = stoi(name_value[1]);
//		stringUtil.SplitString(rule_support_confidence[2], name_value, ":");
//		double confidence = stod(name_value[1]);
//
//		this->put(P, Q,support,confidence);
//	}
//}


// 找到P所在的下标，如果没有就返回-1
//int PrefetchRules::findFrontPiece(vector<unsigned int> P)
//{
//	SetUtil util;
//
//	for (int i = 0; i < this->rules.size(); i++)
//	{
//		if (util.isEqual(P, rules[i].P))
//		{
//			return i;
//			break;
//		}
//	}
//	return -1;
//}



