#include "PreProcessFile.h"



PreProcessFile::PreProcessFile()
{
}


PreProcessFile::~PreProcessFile()
{
}


// 将一列的文件数据转换成PLWAP算法可接受的文件格式
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
