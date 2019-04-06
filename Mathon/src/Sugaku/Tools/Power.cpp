#include "Tools.h"
#include "Complex.h"
#include "PowerObject.h"
#include <cmath>
#include <iostream>
using namespace std;

const Integer pow(const Integer &base, const Integer &e)
{
    if(e == 0) return 1;
    else if(e == 1) return base;
    else if(e < 0) return 1/ pow(base, -e);
    //else
    
    if(e.isEven())
    {
        Integer ret(pow(base, e/2));
        return ret * ret;
    }
    
    //e is odd
    Integer ret(pow(base, (e-1)/2));
    return ret * ret * base;
}

namespace{
    const Decimal pow_helper(const Decimal &base, const Integer &e)
    {
        if(e == 0) return 1;
        else if(e == 1) return base;
        //else
        if(e.isEven())
        {
            Decimal ret(pow_helper(base, e/2));
            return ret * ret;
        }
        
        //e is odd
        Decimal ret(pow_helper(base, (e-1)/2));
        return ret * ret * base;        
    }
}

const Decimal pow(const Decimal &base, const Decimal &e)
{
    if(e < 0) return 1/ pow(base, -e);
    else if(e % 0.5 != 0) throw runtime_error("InternalError: pow has only supported integer & 0.5(sqrt) as exponent currently.");
    else if (e % 1 == 0.5) return pow_helper(base, Integer(e)) * sqrt(base);
    else return pow_helper(base, Integer(e));
}

namespace{
    const Complex pow_helper(const Complex &base, const Integer &e)
    {
        if(e == 0) return 1;
        else if(e == 1) return base;
        //else
        if(e.isEven())
        {
            Complex ret(pow_helper(base, e/2));
            return ret * ret;
        }
        
        //e is odd
        Complex ret(pow_helper(base, (e-1)/2));
        return ret * ret * base;        
    }
}

const Complex pow(const Complex &base, const Complex &e)
{
    if(e < 0) return 1/ pow(base, -e);
    else if(e.hasComplex()) throw runtime_error("InternalError: pow doesn't support complex exponent currently.");
    else if(Decimal(e) % 0.5 != 0) throw runtime_error("InternalError: pow has only supported integer & 0.5(sqrt) as exponent currently.");
    else if (Decimal(e) % 1 == 0.5) return pow_helper(base, Integer(e)) * sqrt(base);
    else return pow_helper(base, Integer(e));
}

//Square root by using digit by digit method.
const Integer sqrt(const Integer &base_)
{   
    if(base_ < 0) throw runtime_error("MathError: sqrt by negative value.");
    string sVal;
    {
        Integer base(base_);
        base.setSign(0);
        sVal = base.str();
        if(sVal.size() % 2 == 1) sVal.insert(sVal.begin(), '0');
    }
    int L, R, N = 0;
    Integer dividend, mul, tmp;
    string dividendStr, mulStr(1,'0');
    for(size_t i = 0, j ; i < sVal.size() ;)
    {
        {
            dividendStr = dividend.str();
            for(j = 0; j < 2; ++j)  dividendStr.push_back(sVal[i++]);
            dividend.set(dividendStr);
        }
        //cout<<dividend<<" DIV"<<endl;
        mul = Integer(mulStr) * 10 + N * 2;
        mulStr = mul.str();
        //cout<<mulStr<<" MUL"<<endl;
        mulStr.push_back('0');
        
        L = 0; R = 10; //L inclusive, R exclusive
        char &mulBack = mulStr.back();
        while(L != R)
        {
            N = (L+R)/2;
            mulBack = N + '0'; 
            mul = Integer(mulStr);
            tmp = N * mul;
            if(tmp > dividend)
            {
                R = N;
            }else{
                if(L == N) break;
                L = N;
            }
        }
        mulStr.pop_back();
        dividend -= tmp;
        //cout<<N<<" N   "<<dividend<< " DIV"<<"     "<<mul<<" MUL"<<endl<<endl;;
    }
    mul = Integer(mulStr);
    mul /= 2;
    mulStr = mul.str();
    mulStr.push_back(N + '0');
    return Integer(mulStr);
}

const Decimal sqrt(const Decimal &base_)
{
    if(base_ < 0) throw runtime_error("MathError: sqrt by negative value.");
    int length = 300 + base_.str().length();
    if(length % 2 == 1) ++length;
    Integer base(base_ *pow(Integer(10), length));
    return Decimal(sqrt(base), pow(Integer(10), length/2));
}

const Complex sqrt(const Complex &base_)
{
    if(base_.hasComplex()) throw runtime_error("InternalError: complex number doesn't support squaring currently.");
    return Complex(Decimal(base_));
}

const PowerObject sqrt(const PowerObject &base)
{
    PowerObject ret(base);
    ret.exp_ /= make_shared<Decimal>(2);
    return ret;
}
//
//Square root by binary division.
/*
const Integer sqrt2(const Integer &base)
{
    if(base < 0)
    {
        cout<<"SQRT NEG NUMBER"<<endl;
    }
    else if(base == 0) return 0;
    //cout<<base.size()<<endl;;
    Integer L = (pow(Integer(MOD), (base.size() / 2) - 1)), R = 4 * (pow(Integer(MOD), (base.size() + 1) / 2));
    //cout<<L.size()<<' '<< R.size()<<endl;;
    Integer N, tmp;
    //cout<<L<<"   "<<R<<endl;
    while(L != R)
    {
        N = (L + R) / 2;
        tmp = N * N;
        if(tmp == base) break;
        else if(tmp < base)
        {
            if(L == N) L = N + 1;
            else L = N;
        }
        else R = N;
        //cout<<N<<endl;
    }
    return N;
}
*/