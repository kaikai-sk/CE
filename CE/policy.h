#ifndef HEADER_POLICY
#define HEADER_POLICY


#define POLICY_IGNORE  0
#define POLICY_APPEND  1
#define POLICY_REPLACE 2
#define POLICY_APPEND_IGNORE  3
#define POLICY_APPEND_APPEND  4
#define POLICY_APPEND_REPLACE  5
#define POLICY_REPLACE_IGNORE  6
#define POLICY_REPLACE_APPEND  7//这种情况不可能出现
#define POLICY_REPLACE_REPLACE  8
#define POLICY_IGNORE_IGNORE  9
#define POLICY_IGNORE_APPEND  10
#define POLICY_IGNORE_REPLACE  11
#define POLICY_PREFETCH_APPEND  12
#define POLICY_PREFETCH_REPLACE  13

#include <vector>
using namespace std;

typedef struct _Action
{
    unsigned type;
    unsigned old_address;
    unsigned new_address;
	unsigned prefetch_address;//根据预取规则预取的地址
	unsigned prefetch_old_address;//预取替换掉的地址
} Action;


class Policy
{
public:
    virtual vector<Action*> hit(unsigned address) = 0;
    virtual vector<Action*> miss(unsigned address) = 0;

    virtual ~Policy() {}
};


#endif
