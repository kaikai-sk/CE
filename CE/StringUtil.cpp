#include "StringUtil.h"



StringUtil::StringUtil()
{
}


StringUtil::~StringUtil()
{
}

void StringUtil::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& separater)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(separater);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + separater.size();
		pos2 = s.find(separater, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

// ��str�ַ���spl�ָ�,����dst�У����������ַ�������
int split(char dst[][80], char* str, const char* spl)
{
	int n = 0;
	char *result = NULL;
	result = strtok(str, spl);
	while (result != NULL)
	{
		strcpy(dst[n++], result);
		result = strtok(NULL, spl);
	}
	return n;
}
