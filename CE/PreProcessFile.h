#pragma once

#include <iostream>
#include <fstream>
using namespace std;

class PreProcessFile
{
public:
	PreProcessFile();
	~PreProcessFile();

	// ��һ�е��ļ�����ת����PLWAP�㷨�ɽ��ܵ��ļ���ʽ
	void vectToMatrix(char* dataFileName, char* dstFileName,int window_size);
};

