#include "PreProcessFile.h"



PreProcessFile::PreProcessFile()
{
}


PreProcessFile::~PreProcessFile()
{
}


// ��һ�е��ļ�����ת����PLWAP�㷨�ɽ��ܵ��ļ���ʽ
void PreProcessFile::vectToMatrix(char* dataFileName, char* dstFileName,int window_size)
{
	ifstream ifs(dataFileName);
	ofstream ofs(dstFileName);

	unsigned address;
	unsigned lineNum = 0;
	unsigned id = 0;
	unsigned *buffer = new unsigned[window_size];

	for (ifs >> address; ifs.good(); ifs >> address)
	{
		buffer[lineNum%window_size] = address;

		lineNum++;
		if (lineNum%window_size == 0)
		{
			id++;
			ofs << id << ' ' << window_size << ' ';
			for (int i = 0; i < window_size-1; i++)
			{
				ofs << buffer[i] << '\t';
			}
			ofs << buffer[window_size - 1] << endl;
		}
	}
}
