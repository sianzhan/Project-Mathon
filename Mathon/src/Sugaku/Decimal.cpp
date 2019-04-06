#include "Integer.h"
#include "Decimal.h"
#include "Compute.h"
#include "Tools.h"
#include <string>
#include <algorithm>

using namespace std;

Decimal::Decimal() : num_(0), den_(1) {}

Decimal::Decimal(const int i) : num_(i), den_(1) {}
Decimal::Decimal(const long long ll) : num_(ll), den_(1) {}
Decimal::Decimal(const long long num, const long long den) : num_(num), den_(den) {}
Decimal::Decimal(const double f) : Decimal(to_string(f)) {}
Decimal::Decimal(const char* input) : Decimal() { this->set(input); }
Decimal::Decimal(const std::string& input) : Decimal() { this->set(input); }

Decimal::Decimal(const Integer& num) : num_(num), den_(1) {}
Decimal::Decimal(const Integer& num, const Integer& den) : num_(num), den_(den) {}
Decimal::Decimal(const Decimal& d) : num_(d.num_), den_(d.den_) { this->clearTrailingZero(); }

Decimal& Decimal::operator=(const Decimal& d) 
{
    if(this == &d) return *this;
    this->num_ = d.num_;
    this->den_ = d.den_;
    this->clearTrailingZero();
    return *this;
}

Decimal::operator Integer() const
{
    string buf = this->str();
    size_t pos = buf.find('.');
    return Integer(string(buf.begin(), buf.begin() + pos));
}


//Mutator function
void Decimal::set(const std::string& input)
{
    string::const_iterator it = input.begin(), itPoint = input.end();
    if(it != input.end() && (*it == '+' || *it == '-')) ++it;
    while(it != input.end())
    {
        if(!isdigit(*it))
        {
            if(*it == '.' && itPoint == input.end()) itPoint = it; //makesure only one decimal point
            else break;
        }
        ++it;
    }
    if(it == input.end()) //completed verification.
    {
        this->den_ = 1;
        if(itPoint != input.end())
        {
            this->num_ = Integer(string(input.begin(), itPoint) + string(itPoint + 1, input.end()));
            this->den_ = Integer(1).shift(input.end() - itPoint - 1);
        }
        else this->num_ = Integer(input);
    }
    else //break in middle, contains invalid character
    {
		try {
			*this = (Decimal)Compute().solve(input);
		}
		catch (exception &e)
		{
			*this = 0;
		}
    }
    if(this->den_.isNeg()) this->den_.setSign(0), this->num_.setSign(!this->num_.isNeg());
    this->clearTrailingZero();
}

void Decimal::setSign(bool sign)
{
    this->num_.setSign(sign);
}

void Decimal::clearTrailingZero()
{
    string sNum = this->num_.str();
    string sDen = this->den_.str();
    int minSize = min(sNum.length(), sDen.length());
    int ptCut = 0;
    while(minSize > 1 && sNum[sNum.length() - ptCut - 1] == '0' && sDen[sDen.length() - ptCut - 1] == '0') ++ptCut, --minSize;
    this->num_.shift(-ptCut);
    this->den_.shift(-ptCut);
}

void Decimal::simplify()
{
    Integer d = gcd(this->num_, this->den_);
    this->num_ /= d;
    this->den_ /= d;
}

const Decimal operator+(const Decimal &lhs_, const Decimal &rhs_)
{
    Decimal lhs(lhs_), rhs(rhs_), ret;
    ret.den_ = lcm(lhs.den_, rhs.den_);
    Integer rlhs(ret.den_ / lhs.den_), rrhs(ret.den_ / rhs.den_); //ratio of lcm to old denominator
    ret.num_ = lhs.num_ * rlhs + rhs.num_ * rrhs;
    return ret;
}

const Decimal operator-(const Decimal &lhs_, const Decimal &rhs_)
{
    Decimal lhs(lhs_), rhs(rhs_), ret;
    ret.den_ = lcm(lhs.den_, rhs.den_);
    Integer rlhs(ret.den_ / lhs.den_), rrhs(ret.den_ / rhs.den_); //ratio of lcm to old denominator
    ret.num_ = lhs.num_ * rlhs - rhs.num_ * rrhs;
    return ret;
}

const Decimal operator*(const Decimal &lhs, const Decimal &rhs)
{
    Decimal ret;
    ret.num_ = lhs.num_ * rhs.num_;
    ret.den_ = lhs.den_ * rhs.den_;
    string buf = ret.str(150);
    size_t posPoint = buf.find('.'), pos;
    for(pos = posPoint + 1; pos < buf.length() && buf[pos] == '9'; ++pos);
    if(pos - posPoint - 1 == 150) ret = (Decimal(Integer(ret) + 1));
    return ret;
}

const Decimal operator/(const Decimal &lhs, const Decimal &rhs)
{
    Decimal ret;
    ret.num_ = lhs.num_ * rhs.den_;
    ret.den_ = lhs.den_ * rhs.num_;
    if(ret.den_.isNeg()) ret.den_.setSign(0), ret.num_.setSign(!ret.num_.isNeg());
    return ret;
}

const Decimal operator%(const Decimal &lhs, const Decimal &rhs)
{
    return lhs - Integer(lhs/rhs) * rhs;
}


const Decimal& Decimal::operator++()
{
    (*this) += 1;
    return *this;
}

const Decimal Decimal::operator++(int)
{
    Decimal ret = *this;
    ++(*this);
    return ret;
}

const Decimal& Decimal::operator--()
{
    (*this) -= 1;
    return *this;
}

const Decimal Decimal::operator--(int)
{
    Decimal ret = *this;
    --(*this);
    return ret;
}

//Comparator
bool operator==(const Decimal &lhs_, const Decimal &rhs_)
{
    Decimal lhs(lhs_), rhs(rhs_);
    lhs.simplify(), rhs.simplify();
    return lhs.num_ == rhs.num_ && lhs.den_ == rhs.den_;
}
bool operator<(const Decimal &lhs, const Decimal &rhs) { return (lhs - rhs).isNeg(); }
bool operator>(const Decimal &lhs, const Decimal &rhs) { return (rhs - lhs).isNeg(); }
bool operator!=(const Decimal &lhs, const Decimal &rhs) { return !(lhs == rhs); }
bool operator<=(const Decimal &lhs, const Decimal &rhs) { return lhs < rhs || lhs == rhs; }
bool operator>=(const Decimal &lhs, const Decimal &rhs) { return lhs > rhs || lhs == rhs; }

const bool Decimal::isInteger() const
{
    (const_cast<Decimal*>(this))->simplify();
    return this->den_ == 1;
}

const std::string Decimal::str(int precision) const
{
    Integer ret(this->num_);
    ret.shift(precision);
    ret /= this->den_;
    bool neg = ret.isNeg();
    ret.setSign(0);
    string buf(ret.str());
    if((int)buf.length() <= precision) buf.insert(buf.begin(), precision - buf.length() + 1, '0');
    buf.insert(buf.end() - precision, '.');
    if(neg) buf.insert(buf.begin(), '-');
    return buf;
}

void Decimal::read(istream& is)
{
    string buf;
    while(isspace(is.peek())) is.get();
    while(is.peek() == '+' || is.peek() == '-') buf.push_back(is.get());
    while(isdigit(is.peek())) buf.push_back(is.get());
    if(is.peek() == '.')
    {
        buf.push_back(is.get());
        while(isdigit(is.peek())) buf.push_back(is.get());
    }
    *this = Decimal(buf);
}