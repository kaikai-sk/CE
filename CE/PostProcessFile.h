#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
using namespace std;

class PostProcessFile
{
public:
	PostProcessFile();
	~PostProcessFile();


	// ��Ƶ�������ھ���ļ��еĶ�Ԫ��ϵ�ҳ���
	void findBinaryRules(char* dataFileName,char* dstFileName);
	// ���ɶ�Ԫ�Ĺ���
	void generateBinaryRules(char* dataFileName, char* dstFileName);
};

