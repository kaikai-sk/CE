#include <vector>
#include <fstream>
#include "p_policy.h"


class FeatureTable
{
    double theta, yita;
	//const unsigned hsize=4;
    unsigned hsize,fsize, tsize;
    unsigned tail;
    std::vector<unsigned> history;
    std::vector<std::vector<double>> weights;
private:
    void update_history(unsigned address);
    void get_features(unsigned features[]);
public:
    FeatureTable();
    FeatureTable(
        double theta, double yita,
        unsigned history_size, unsigned feature_size, unsigned table_size);

    void set(unsigned address, bool result);
    bool get(unsigned address);
    void load(void);
    void store(void);
};

FeatureTable::FeatureTable() {}

FeatureTable::FeatureTable(
    double theta, double yita,
    unsigned history_size = 4,
    unsigned feature_size = 6,
    unsigned table_size = 256
)
    : theta(theta), yita(yita),
      hsize(history_size), fsize(feature_size), tsize(table_size), tail(0)
{
    history.resize(hsize, 0);
    weights.resize(fsize);
    for(auto &table : weights)
        table.resize(tsize, 0.5);
}

void FeatureTable::update_history(unsigned address)
{
    tail = tail + 1;
    tail = tail % hsize;
    history[tail] = address;
}

void FeatureTable::get_features(unsigned features[])
{
    //unsigned h[hsize];
	unsigned* h = new unsigned[hsize];

    for(unsigned i = 0; i < hsize; i++)
        h[i] = history[(tail + hsize - i) % hsize];

    features[0] = (h[0] >> 2) ^ h[0];
    features[1] = (h[1] >> 1) ^ h[0];
    features[2] = (h[2] >> 2) ^ h[0];
    features[3] = (h[3] >> 3) ^ h[0];
    features[4] = (h[0] >> 4) ^ h[0];
    features[5] = (h[0] >> 7) ^ h[0];

    for(unsigned i = 0; i < fsize; i++)
        features[i] &= tsize - 1;
}

void FeatureTable::set(unsigned address, bool result)
/* offline train
 * 'result == true' means reuse */
{
    double y_out;
    //unsigned features[fsize];
	unsigned* features = new unsigned[fsize];

    update_history(address);
    get_features(features);

    y_out = 0;
    for(unsigned i = 0; i < fsize; i++)
        y_out += weights[i][features[i]];

    if(result)
    {
        while((y_out > theta) == result)
        {
            for(unsigned i = 0; i < fsize; i++)
                weights[i][features[i]] -= yita * weights[i][features[i]];

            y_out = 0;
            for(unsigned i = 0; i < fsize; i++)
                y_out += weights[i][features[i]];
        }
    }
    else
    {
        while((y_out > theta) == result)
        {
            for(unsigned i = 0; i < fsize; i++)
                weights[i][features[i]] += yita * weights[i][features[i]];

            y_out = 0;
            for(unsigned i = 0; i < fsize; i++)
                y_out += weights[i][features[i]];
        }
    }
}

bool FeatureTable::get(unsigned address)
/* return true if reuse */
{
    double y_out = 0;
   // unsigned features[fsize];
	unsigned *features = new unsigned[fsize];

    update_history(address);
    get_features(features);

    for(unsigned i = 0; i < fsize; i++)
        y_out += weights[i][features[i]];

    return y_out <= theta;
}

void FeatureTable::load(void)
{
    std::fstream fs("p_policy.conf", std::ios_base::in);

    fs >> theta >> yita;
    fs >> hsize >> fsize >> tsize;

    tail = 0;
    history.resize(hsize, 0);
    weights.resize(fsize);

    for(unsigned i = 0; i < fsize; i++)
    {
        weights[i].resize(tsize);
        for(unsigned j = 0; j < tsize; j++)
        {
            fs >> weights[i][j];
        }
    }

    fs.close();
}

void FeatureTable::store(void)
{
    std::fstream fs("p_policy.conf", std::ios_base::out);

    fs << theta << std::endl;
    fs << yita << std::endl;

    fs << hsize << std::endl;
    fs << fsize << std::endl;
    fs << tsize << std::endl;

    for(unsigned i = 0; i < fsize; i++)
    {
        for(unsigned j = 0; j < tsize; j++)
        {
            fs << weights[i][j] << std::endl;
        }
    }

    fs.close();
}


class PPolicy: public Policy
{
public:
};


#ifdef __DEBUG_p_policy__
// g++ -D__DEBUG_p_policy__ -Wall lru_core.cpp p_policy.cpp
// ./a.out train theta yita cache_size input_file
// ./a.out train 2 0.1 256 input.txt
#include <cstring>
#include <iostream>
#include <unordered_map>
#include "lru_core.h"
using namespace std;
int train(int argc, char *argv[])
{
    if(argc != 4)
    {
        cout << "arg number invalid" << endl;
        return -1;
    }

    unsigned theta = atof(argv[0]);
    unsigned yita  = atof(argv[1]);
    if(theta == 0.0 || yita == 0.0)
    {
        cout << "learning arg invalid" << endl;
        return -1;
    }

    unsigned csize = atoi(argv[2]);
    if(!csize)
    {
        cout << "cache size invalid" << endl;
        return -1;
    }

    fstream fs(argv[3]);
    if(fs.bad())
    {
        cout << "cannot open input file" << endl;
        return -1;
    }

    FeatureTable ft(theta, yita);
#if 0
    unsigned address;

    fs >> address;
    while(fs.good())  // eof test
    {
        if(reuse.find(address) != reuse.end())
        {
            reuse[address]++;
        }
        else
        {
            reuse[address] = 1;
        }

        fs >> address;
    }

    fs.seekp(0);

    fs >> address;
    while(fs.good())  // eof test
    {
        if(reuse[address])
        {
            reuse[address]--;

            ft.set(address, true);
        }
        else
        {
            ft.set(address, false);
        }

        fs >> address;
    }
#endif
    fs.close();
    ft.store();
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc >= 2)
    {
        if(!strcmp(argv[1], "train"))
            return train(argc - 2, argv + 2);
        if(!strcmp(argv[1], "test"))
            return 0;
    }

    return 0;
}
#endif
