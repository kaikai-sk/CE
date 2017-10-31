#ifndef HEADER_POLICY
#define HEADER_POLICY


#define POLICY_IGNORE  0
#define POLICY_APPEND  1
#define POLICY_REPLACE 2


typedef struct _Action {
    unsigned type;
    unsigned old_address;
    unsigned new_address;
} Action;


class Policy
{
public:
    virtual Action &hit(unsigned address) = 0;
    virtual Action &miss(unsigned address) = 0;

    virtual ~Policy() {}
};


#endif
