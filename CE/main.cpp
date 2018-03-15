#include <iostream>
#include <fstream>
#include <cstdlib>

#include "lru_policy.h"
#include "cache.h"
#include "plwap.h"
#include "PreProcessFile.h"
#include "PostProcessFile.h"
using namespace std;

/*
	argv[0]: exe文件的文件名
	argv[1]: trace文件路径
	argv[2]: cache大小
	argv[3]: 是否打开plwap算法 频发序列挖掘的功能
	argv[4]: 是否启用统计unique page的个数，以及确定cache的大小
*/
int main(int argc, char *argv[])
{
    if(argc != 5)
    {
        cout << argv[0] << " file capacity" << endl;
		cin.get();
        return 0;
    }

	//如果打开plwap开关，我们就执行PLWAP挖掘
	if (strcmp(argv[3],"1")==0)
	{
		cout << argv[1] << endl;
		PreProcessFile preProcessFile;
		preProcessFile.vectToMatrix(argv[1], ".\\temp\\dstForPLWAP.data", 10);
		plwap(0.5,".\\temp\\result_PLWAP.data",".\\temp\\dstForPLWAP.data");
		/*plwap(0.6, "result_PLWAP.data", "example.data");*/
		PostProcessFile postProcessFile;
		postProcessFile.generateBinaryRules(".\\temp\\result_PLWAP.data", ".\\dst\\result_PLWAP_post.data");
		cin.get();
		cin.get();
		return EXIT_SUCCESS;
	}
	//运行RuleGrowth算法
	else if (strcmp(argv[3], "2") == 0)
	{

	}

	//统计unique page的出现次数，以及确定cache的大小
	if (strcmp(argv[4], "1") == 0)
	{
		ifstream ifs(argv[1]);
		unsigned address;
		map<unsigned, unsigned> table;

		int line_num = 0;

		for (ifs >> address; ifs.good(); ifs >> address)
		{
			line_num++;
			if (line_num == 10000)
			{
				break;
			}
			map<unsigned, unsigned>::iterator iter;
			iter = table.find(address);
			if (iter != table.end())
			{
				iter->second++;
			}
			else
			{
				table.insert(pair<unsigned, unsigned>(address, 1));
			}
		}

		int unique_num= table.size();
		cout << "the number of unique page:" << unique_num<<endl;
		cout << "10%情况下的cache大小" << unique_num*0.1<<endl;
		cout << "20%情况下的cache大小" << unique_num*0.2<<endl;

		cin.get();

		return EXIT_SUCCESS;
	}


    ifstream ifs(argv[1]);
	ofstream ofs(".\\temp\\accessSeq.txt");
	ofstream ofs_cache_sanpshoot(".\\temp\\cache_snapshoot.txt");
	ofstream ofs_lruCore_snapshot(".\\temp\\lru_core_snapshoot.txt");
	ofstream ofs_page_detail(".\\temp\\page_detail.txt");

    unsigned capacity = atol(argv[2]);

    cout << "file: " << argv[1] << " / capacity: " << capacity << endl;

	PrefetchRules prefetchRules;
	prefetchRules.head = NULL;

	//prefetchRules.loadRules(".\\source\\ts01\\rules_0.025_0.5.txt");
	loadRules(&prefetchRules, ".\\source\\artificial\\test.data");
	//printRules(&prefetchRules);

    Policy *policy = new LRUPolicy(capacity,prefetchRules,NULL,&ofs_lruCore_snapshot,&ofs_page_detail);
    Cache cache(capacity, policy,&ofs,&ofs_cache_sanpshoot);

    unsigned address;
    unsigned hit_count = 0;
    unsigned miss_count = 0;


	int lineNum = 0;
	int threshold = 10000;

    for(ifs >> address; ifs.good(); ifs >> address)
    {
		lineNum++;
	/*	if (lineNum <= threshold)
		{
			continue;
		}*/
		if (lineNum % 500==0)
		{
			cout << lineNum << endl;
		}
		if (lineNum == threshold)
		{
			break;
		}
        if(cache.query(address) == CACHE_HIT)
            hit_count++;
        else
            miss_count++;

    }
	
    cout << "hit:" << hit_count << " / miss:" << miss_count << endl;
    cout << "hit ratio: " << (100.0 * hit_count / (hit_count + miss_count)) << "%" << endl;
    cout << "miss ratio: " << (100.0 * miss_count / (hit_count + miss_count)) << "%" << endl;
	cout << "replace num:" << cache.getReplaceNum() << endl;
	cout << "prefetch num:" << ((LRUPolicy*)policy)->getPrefetchCount() << endl;
	cout << "prefetch replace num:" << ((LRUPolicy*)policy)->getPrefetchReplaceCount() << endl;
	((LRUPolicy*)policy)->statistic();

	cin.get();

	delete policy;
	ifs.close();

    return 0;
}
