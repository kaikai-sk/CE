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
    using std::cout;
    using std::endl;
    using std::ifstream;

    if(argc != 5)
    {
        cout << argv[0] << " file capacity" << endl;
		cin.get();
        return 0;
    }

	//如果打开plwap开关，我们就执行PLWAP挖掘
	if (strcmp(argv[3],"1")==0)
	{
		PreProcessFile preProcessFile;
		preProcessFile.vectToMatrix(argv[1], "dstForPLWAP.data", 20);
		plwap(0.05,"result_PLWAP.data","dstForPLWAP.data");
		/*plwap(0.6, "result_PLWAP.data", "example.data");*/
		PostProcessFile postProcessFile;
		postProcessFile.generateBinaryRules("result_PLWAP.data", "result_PLWAP_post.data");
		cin.get();
		return EXIT_SUCCESS;
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
	ofstream ofs("accessSeq.txt");
	ofstream ofs_cache_sanpshoot("cache_snapshoot.txt");
	ofstream ofs_lruCore_snapshot("lru_core_snapshoot.txt");
	ofstream ofs_page_detail("page_detail.txt");

    unsigned capacity = atol(argv[2]);

    cout << "file: " << argv[1] << " / capacity: " << capacity << endl;

	ifstream prefetchRulesFile("result_PLWAP_post.data");
	unsigned P, Q;
	PrefetchRules prefetchRules;

	for (prefetchRulesFile >> P >> Q; prefetchRulesFile.good(); prefetchRulesFile >> P >> Q)
	{
		prefetchRules.put(P, Q);
	}

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
		if (lineNum % 1000==0)
		{
			cout << lineNum << endl;
		}
		if (lineNum == 10000)
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

	cin.get();

	delete policy;
	ifs.close();

    return 0;
}
