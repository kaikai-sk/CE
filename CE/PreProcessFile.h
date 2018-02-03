#pragma once

#include <iostream>
#include <fstream>
using namespace std;

class PreProcessFile
{
public:
	PreProcessFile();
	~PreProcessFile();

	// 将一列的文件数据转换成PLWAP算法可接受的文件格式
	void vectToMatrix(char* dataFileName, char* dstFileName,int window_size);
};

