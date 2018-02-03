#include "PostProcessFile.h"



PostProcessFile::PostProcessFile()
{
}


PostProcessFile::~PostProcessFile()
{
}


// ��Ƶ�������ھ���ļ��еĶ�Ԫ��ϵ�ҳ���
void PostProcessFile::findBinaryRules(char* dataFileName,char* dstFileName)
{
	ifstream fin(dataFileName, ios::in);
	ofstream ofs(dstFileName);

	char line[1024] = { 0 };
	vector<unsigned> events;
	while (fin.getline(line, sizeof(line)))
	{
		stringstream word(line);
		unsigned address;
		while (word >> address)
		{
			events.push_back(address);
		}
		if (events.size() == 2)
		{
			ofs << events[0] << ' ' << events[1] << endl;
			events.clear();
		}
		events.clear();
	}

	fin.clear();
	fin.close();
	ofs.clear();
	ofs.close();
}


// ���ɶ�Ԫ�Ĺ���
void PostProcessFile::generateBinaryRules(char* dataFileName, char* dstFileName)
{
	ifstream fin(dataFileName, ios::in);
	ofstream ofs(dstFileName);

	char line[1024] = { 0 };

	queue<unsigned> rules;
	unsigned eventsNum = 0;

	while (fin.getline(line, sizeof(line)))
	{
		stringstream word(line);
		unsigned address;
		while (word >> address)
		{
			if (rules.size() >= 2)
			{
				rules.pop();
			}
			rules.push(address);
			if (rules.size() == 2)
			{
				unsigned add1, add2;
				add1 = rules.front();
				rules.pop();
				add2 = rules.front();
				rules.pop();

				rules.push(add1);
				rules.push(add2);
				
				ofs << add1 << ' ' << add2 << endl;
			}
		}
		//���rules
		while (!rules.empty())
		{
			rules.pop();
		}
		eventsNum = 0;
	}

	fin.clear();
	fin.close();
	ofs.clear();
	ofs.close();
}
