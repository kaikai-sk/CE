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


	// 将频繁序列挖掘的文件中的二元关系找出来
	void findBinaryRules(char* dataFileName,char* dstFileName);
	// 生成二元的规则
	void generateBinaryRules(char* dataFileName, char* dstFileName);
};

