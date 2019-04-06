#include "Tools.h"
#include "Integer.h"
#include "Decimal.h"
#include "Complex.h"
using namespace std;

/*
namespace
{
    Integer countExp(const Integer &z, const Integer &p)
    {
        Integer count(0), tmp(z);
        while(tmp >= p)
        {
            tmp /= p;
            count += tmp;
        }
        return count;
    }
}

//Factorial using prime factorization.

const Integer fact(const Integer &level)
{
    if(level <= 1) return 0;
    Integer ret = 1;
    //Multiplying factorial from the back will be much mooooooore faster!!!!!!!!!!
    for(Integer i = level; i >= 2; --i)
    {
        if(isPrime(i)){
            //cout<<ret<<"   "<<i<<" : "<<countExp(level, i)<<"    ";
            //cout << i <<endl;
            //cout<<pow(i, countExp(level, i))<<endl;
            ret *= pow(i, countExp(level, i));
            //cout<<ret<<endl;
        }
    }
    
    return ret;
}*/


//Factorial using binary divide & conquer

const Integer fact_helper(const Integer &L, const Integer &R)
{
    if(L == R) return L;
    Integer med = (L + R) / 2;
    return fact_helper(L, med) * fact_helper(med + 1, R);
}

const Integer fact(const Integer &level)
{
    if(level < 0) throw runtime_error("MathError: factorial can only be non-negative integer.");
    else if(level < 2) return 1;
    return fact_helper(2, level);
}

const Decimal fact(const Decimal &level)
{
    if(level < 0 || !level.isInteger()) throw runtime_error("MathError: factorial can only be non-negative integer.");
    else if(level < 2) return 1;
    return fact_helper(2, Integer(level));
}

const Complex fact(const Complex &level)
{
    if(level.hasComplex()) throw runtime_error("MathError: factorial can't be complex number.");
    return fact(Decimal(level));
}