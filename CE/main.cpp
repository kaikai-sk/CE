#include <iostream>
#include <fstream>
#include <cstdlib>

#include "lru_policy.h"
#include "cache.h"
using namespace std;


int main(int argc, char *argv[])
{
    using std::cout;
    using std::endl;
    using std::ifstream;

    if(argc != 3)
    {
        cout << argv[0] << " file capacity" << endl;
		cin.get();
        return 0;
    }

    ifstream ifs(argv[1]);
	ofstream ofs("accessSeq.txt");
    unsigned capacity = atol(argv[2]);

    cout << "file: " << argv[1] << " / capacity: " << capacity << endl;

	ifstream prefetchRulesFile("result_PLWAP_post.data");
	unsigned P, Q;
	PrefetchRules prefetchRules;

	for (prefetchRulesFile >> P >> Q; prefetchRulesFile.good(); prefetchRulesFile >> P >> Q)
	{
		prefetchRules.put(P, Q);
		//break;
	}



    Policy *policy = new LRUPolicy(capacity,prefetchRules,&ofs);
    Cache cache(capacity, policy);

    unsigned address;
    unsigned hit_count = 0;
    unsigned miss_count = 0;


	int lineNum = 0;
	int threshold = 100000;

    for(ifs >> address; ifs.good(); ifs >> address)
    {
		lineNum++;
		if (lineNum <= threshold)
		{
			continue;
		}

        if(cache.query(address) == CACHE_HIT)
            hit_count++;
        else
            miss_count++;
    }

    delete policy;
    ifs.close();

    cout << "hit:" << hit_count << " / miss:" << miss_count << endl;
    cout << "hit ratio: " << (100.0 * hit_count / (hit_count + miss_count)) << "%" << endl;
    cout << "miss ratio: " << (100.0 * miss_count / (hit_count + miss_count)) << "%" << endl;
	cout << "replace num:" << cache.getReplaceNum() << endl;

	cin.get();

    return 0;
}
