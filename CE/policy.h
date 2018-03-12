#ifndef HEADER_POLICY
#define HEADER_POLICY

#include <vector>
using namespace std;

class Policy
{
public:
    virtual void hit(unsigned address) = 0;
    virtual void miss(unsigned address) = 0;

    virtual ~Policy() {}
};

#endif
