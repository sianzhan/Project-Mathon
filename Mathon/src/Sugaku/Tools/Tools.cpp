#include "Tools.h"
#include "Integer.h"
#include <algorithm>
#include <memory>

using namespace std;

const Integer gcd(const Integer &lhs_, const Integer &rhs_)
{
    unique_ptr<Integer> lhs(new Integer(lhs_)), rhs(new Integer(rhs_));
    lhs->setSign(0), rhs->setSign(0);
    if(*lhs < *rhs) lhs.swap(rhs);
    
    while(*rhs != 0)
    {
        *lhs %= *rhs;
        lhs.swap(rhs);
    }
    return *lhs;
}

const Integer lcm(const Integer &lhs, const Integer &rhs)
{
    return lhs * (rhs / gcd(lhs, rhs));
}