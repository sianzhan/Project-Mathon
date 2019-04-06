#include "Integer.h"
#include "Compute.h"
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <memory>
#include <regex>

using namespace std;

const static int DIGIT = 9;
const static int MOD = std::pow(10, DIGIT);

//Private Constructor
Integer::Integer(const vector<int> &v, bool sign) : limb_(v){ this->clearLeadingZero(); this->setSign(sign); }
Integer::Integer(const Integer& z, bool sign) : limb_(z.limb_){ this->clearLeadingZero(); this->setSign(sign); }

//Public Constructor
Integer::Integer():limb_(0 ,0), sign_(0){}
Integer::Integer(const int N) { this->set(N); }
Integer::Integer(const long long N) { this->set(N); }
Integer::Integer(const double N) { this->set((long long)N); }
Integer::Integer(const char* input) {this->set(input); }
Integer::Integer(const string &input) { this->set(input); }

Integer::Integer(const Integer& z) : limb_(z.limb_), sign_(z.sign_){ this->clearLeadingZero();}

Integer& Integer::operator=(const Integer& z)
{
    if(&z == this) return *this;
    this->limb_ = z.limb_;
    this->sign_ = z.sign_;
    this->clearLeadingZero();
    return *this;
}
Integer& Integer::operator=(const char *s)
{
    this->set(s);
    return *this;
}

Integer::operator shared_ptr<NumberObject>() { return make_shared<Integer>(*this); }

void Integer::set(long long N)
{
    this->clear();
    bool sign = 0;
    if(N < 0)
    {
        sign = 1;
        N *= -1;
    }
    while(N != 0)
    {
        this->limb_.push_back(N % MOD);
        N /= MOD;
    }
    this->setSign(sign);
}


void Integer::set(const string &input)
{
    if(input.size() == 0){ this->clear(); return; }
    
    bool sign = 0;
    string::const_iterator it = input.begin();
    if(it != input.end() && (*it == '+' || *it == '-')) 
    {
        if(*it == '-') sign = 1;
        ++it;
    }
    const string::const_iterator front = it; //iterator of begin exclude signess;
    
    for(; it != input.end() && isdigit(*it); ++it); //check for every characters is digit.
    
    if(it == input.end()) //passed verification.
    {
        this->clear();
        //Start reading the string from the back to front.
        string::const_iterator left = front, right = input.end();
        while(right != front){
			if (right - front < DIGIT) left = front;
            else left = right - DIGIT;
            this->limb_.push_back(stoi(string(left, right)));
            right = left;
        }
        this->setSign(sign);
    }
    else
    {
		try {
			*this = (Integer)Compute().solve(input);
		}
		catch(exception &e){
			*this = 0;
		}
    }
    this->clearLeadingZero();
}

void Integer::setSign(bool sign)
{
    if(!sign || this->isZero())
    {
        this->sign_ = 0;
        return;
    }
    else if(sign){
        this->sign_ = 1;
    }
}

void Integer::clearLeadingZero()
{
    while(this->size() > 1 && this->limb_[this->size()-1] == 0){
        this->limb_.pop_back();
    }
}
    

const Integer operator+(const Integer &lhs_, const Integer &rhs_)
{
    Integer lhs(lhs_), rhs(rhs_); // Make a copy so they can be complemented(if needed) after.
    bool sign(0), hasComp(0);
    const size_t Size = max(lhs.size(), rhs.size()) + 1;
    //if there's two negative, treat them as normal addition with sign set to '-'', no need to complement them.
    if((lhs.isNeg() && rhs.isNeg())) sign = 1;
    else if(lhs.isNeg() || rhs.isNeg()){
        if(lhs.isNeg()) lhs.complement(Size);
        else rhs.complement(Size);
        hasComp = 1;
    }
    
    Integer ret;
    unsigned long long carry = 0, tmp;
    for(size_t i = 0; i < Size; ++i)
    {
        tmp = carry;
        if(i < lhs.size()) tmp += lhs[i];
        if(i < rhs.size()) tmp += rhs[i];
        ret.limb_.push_back(tmp % MOD);
        carry = tmp / MOD;
    }
    //carry won't exceed 1 if there are no complement (Size is always large enough to hold (non-complemented) two added Integer)
    //so if there's one complement, and carry == 1 means the result is positive. else negative.
    
    if(hasComp && carry == 0)
    {
        ret.complement(Size);
        sign = 1;
    }
    ret.setSign(sign);
    ret.clearLeadingZero();
    return ret;
}

//THIS CANT HANDLE ZERO
void Integer::complement(const size_t& Size)
{
    const static int Ceil = MOD - 1;
    int carry = 1;
    //Complement every cell int by maximum digit;
    for(int &cell : this->limb_)
    {
        cell = Ceil - cell + carry;
        if(carry)
        {
            carry = cell / MOD;
            cell %= MOD;
        }
    }
    //make sure the limb is up to Size by appending.
    if(!carry)
    {
        while(this->size() < Size)
        {
            this->limb_.push_back(Ceil);
        }
    }
}

const Integer operator-(const Integer& lhs, const Integer& rhs)
{
    return lhs + Integer(rhs, !rhs.isNeg());
}

const Integer operator*(const Integer &lhs, const Integer &rhs)
{
    bool sign(0);
    if(lhs.isNeg() || rhs.isNeg())
    {
        if(lhs.isNeg() && rhs.isNeg()) sign = 0;
        else sign = 1;
    }
    Integer ret;
    unsigned long long carry = 0;
    //cout<<lhs.size()<<' '<<rhs.size()<<endl;
    for(size_t i = 0; i < lhs.size(); ++i)
    {
        Integer tmp;
        tmp.limb_.insert(tmp.limb_.begin(), i, 0);
        for(size_t j = 0; j < rhs.size(); ++j)
        {
            carry += (unsigned long long)lhs[i] * rhs[j];
            tmp.limb_.push_back(carry % MOD);
            carry /= MOD;
        }
        while(carry != 0)
        {
            tmp.limb_.push_back(carry % MOD);
            carry /= MOD;            
        }
        ret += tmp;
    }
    ret.setSign(sign);
    ret.clearLeadingZero();
    return ret;
}

//Long Multiplication using binary divide & conquer method
const Integer operator/(const Integer &lhs, const Integer &rhs)
{
    return fmod(lhs, rhs).first;
}


//a%b = a - (a/b) * b;
const Integer operator%(const Integer &lhs, const Integer &rhs)
{
    if(rhs == 2) return !lhs.isEven();
    bool sign(0);
    if(lhs.isNeg()) sign = 1;
    Integer mod = fmod(lhs, rhs).second;
    mod.setSign(sign);
    return mod;
}

const pair<Integer, Integer> fmod(const Integer &lhs_, const Integer &rhs_) //Dividend will eventually become mod
{
    bool sign(0);
    if(lhs_.isNeg() || rhs_.isNeg())
    {
        if(lhs_.isNeg() && rhs_.isNeg()) sign = 0;
        else sign = 1;
    }
    const Integer lhs(lhs_, 0), rhs(rhs_, 0);
    if(rhs.isZero()) throw runtime_error("MathError: divide by 0, undefined bahaviour.");
    else if(lhs < rhs) return pair<Integer, Integer>(0, lhs);
    int chunk = lhs.size() - rhs.size();
    int L, R, N; //Ratio
    Integer dividend = Integer(vector<int>(lhs.limb_.end()-rhs.size() + 1, lhs.limb_.end()));
    Integer tmp;
    vector<int> ret;
    for(int i = 0; i <= chunk; ++i)
    {
        dividend.limb_.insert(dividend.limb_.begin(), lhs[lhs.size() - rhs.size() - i]);
        dividend.clearLeadingZero();
        //cout<<dividend<<endl;
        L = 0; R = MOD;
        while(L != R)
        {
            N = (L+R+1)/2;
            tmp = N * rhs;
            if(tmp > dividend)
            {
                if(N == R) --R;
                else R = N;
            }else{
                L = N;
            }
            //cout<<(tmp > dividend)<<":::"<<tmp<<"     "<<L<<' '<<R<<endl;
        }
        //cout<<dividend<<"  :DIVIDEND"<<endl;
        //cout<<L<<"  :L"<<endl;
        //cout<<rhs<<"  :RHS"<<endl;
        //cout<<L * rhs<<"  : L * RHS"<<endl;
        dividend -= (L * rhs);
        //cout<<dividend<<"  :DIVIDEND_NEW (DIVIDEND - L * RHS)"<<endl<<endl;
        ret.push_back(L);
    }
    return pair<Integer,Integer>(Integer(vector<int>(ret.rbegin(), ret.rend()), sign), dividend); //Dividend is now MOD
}


//Incremental/Decremental 
const Integer& Integer::operator++()
{
    *this = *this + 1;
    return *this;
}

const Integer Integer::operator++(int)
{
    Integer ret = *this;
    ++(*this);
    return ret;
}

const Integer& Integer::operator--()
{
    *this = *this - 1;
    return *this;
}

const Integer Integer::operator--(int)
{
    Integer ret = *this;
    --(*this);
    return ret;
}

bool operator==(const Integer &lhs, const Integer &rhs)
{
    //Compare if they are both zero;
    if(lhs.isZero() && rhs.isZero()) return true;
    
    if(lhs.size() != rhs.size() || lhs.isNeg() != rhs.isNeg()) return false;
    
    {
        for(int i = lhs.size()-1; i >= 0; --i)
        {
            if(lhs[i] != rhs[i]) return false;
        }        
    }
    return true;
}

bool operator<(const Integer &lhs, const Integer &rhs)
{
    //Check for zero case;
    if(rhs.isZero()) 
    {
        if(lhs.isNeg()) return true;
        else return false;
    }
    
    if(lhs.isNeg() && !rhs.isNeg()) return true;
    else if(!lhs.isNeg() && rhs.isNeg()) return false;
    //same sign
    else if(lhs.size() < rhs.size()) return true;
    else if(lhs.size() > rhs.size()) return false;
    //equal size
    else
    {
        for(int i = lhs.size()-1; i >= 0; --i)
        {
            if(lhs[i] < rhs[i]) return true;
            else if(lhs[i] > rhs[i]) return false;
        }
    }
    return false;
}

bool operator>(const Integer &lhs, const Integer &rhs)
{
    //Check for zero case;
    if(lhs.isZero() && rhs.isZero()) return false;
    if(rhs.isZero()) 
    {
        if(lhs.isNeg() || lhs.isZero()) return false;
        else return true;
    }
    
    if(lhs.isNeg() && !rhs.isNeg()) return false;
    else if(!lhs.isNeg() && rhs.isNeg()) return true;
    //same sign
    else if(lhs.size() < rhs.size()) return false;
    else if(lhs.size() > rhs.size()) return true;
    //equal size
    else
    {
        for(int i = lhs.size()-1; i >= 0; --i)
        {
            if(lhs[i] > rhs[i]) return true;
            else if(lhs[i] < rhs[i]) return false;
        }        
    }
    return false;
}

bool operator!=(const Integer &lhs, const Integer &rhs)
{
    if(lhs == rhs) return false;
    return true;
}

bool operator<=(const Integer &lhs, const Integer &rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>=(const Integer &lhs, const Integer &rhs)
{
    return (lhs > rhs) || (lhs == rhs);
}

//Checker Function
const bool Integer::isNumber(const string& s) const
{
    return !s.empty() && find_if(s.begin(), s.end()
    , [](char c) { return !isdigit(c); }) == s.end();
}

const bool Integer::isZero() const
{
    if((this->size() == 0 || (this->size() == 1 && (*this)[0] == 0))) return true;
    return false;
}

const bool Integer::isNeg() const 
{ 
    return this->sign_; 
}

const bool Integer::isEven() const
{
    if(this->size() > 0 && (*this)[0] % 2 != 0) return false;
    return true;
}

const bool Integer::isOdd() const
{
    return !this->isEven();
}

const size_t Integer::size() const 
{ 
    return this->limb_.size(); 
}

const Integer& Integer::shift(int digit)
{
    string str = this->str();
    if(digit > 0)
    {
        str.append(digit, '0');
    }
    else if(digit < 0)
    {
        while(str.size() > 0 && digit < 0)
        {
            str.pop_back();
            digit++;
        }
    }
    this->set(str);
    return *this;
}

const string Integer::str() const
{
    if(this->size() == 0) return string("0");
    string ret, buf;
    if(this->isNeg()) ret.push_back('-');
    if(this->size()) ret.append(to_string(this->limb_[this->size()-1]));
    for(int i = (int)this->size()-2; i >= 0; --i)
    {
        buf = to_string(this->limb_[i]);
        if(DIGIT - (int)buf.length() < 0) throw runtime_error("InternalError: please contact admin. Z.0x03");
        ret.append(DIGIT - buf.length(), '0');
        ret.append(buf);
    }
    return ret;
}

void Integer::read(istream& is)
{
    string buf;
    while(isspace(is.peek())) is.get();
    while(is.peek() == '+' || is.peek() == '-') buf.push_back(is.get());
    while(isdigit(is.peek())) buf.push_back(is.get());
    *this = Integer(buf);
}