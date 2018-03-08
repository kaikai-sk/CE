#include "SetUtil.h"



SetUtil::SetUtil()
{
}


SetUtil::~SetUtil()
{
}

void SetUtil::setAppend(vector<unsigned int>& v1, vector<unsigned int>& v2)
{
	vector<unsigned int>::iterator iter;

	for (int i = 0; i < v2.size(); i++)
	{
		iter = find(v1.begin(), v1.end(), v2[i]);
		if (iter == v1.end())
		{
			v1.push_back(v2[i]);
		}
	}
}

/*
	程序设置判断v2是否为v1的子集
*/
bool SetUtil::isSubset(vector<unsigned int> v1, vector<unsigned int> v2) 
{
	int i = 0, j = 0;
	int m = v1.size();
	int n = v2.size();
	if (m<n) 
	{
		return 0;
	}
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	while (i<n&&j<m) 
	{
		if (v1[j]<v2[i]) 
		{
			j++;
		}
		else if (v1[j] == v2[i]) 
		{
			j++;
			i++;
		}
		else if (v1[j]>v2[i]) 
		{
			return 0;
		}
	}
	if (i<n)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


// 判断两个集合是否完全相同
bool SetUtil::isEqual(vector<unsigned int> P, vector<unsigned int> Q)
{
	if (P.size() != Q.size())
	{
		return false;
	}
	sort(P.begin(), P.end());
	sort(Q.begin(), Q.end());

	for (int i = 0; i < P.size(); i++)
	{
		if (P[i] != Q[i])
		{
			return false;
			break;
		}
	}

	return true;
}
