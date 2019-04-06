#include "Integer.h"
#include "Decimal.h"
#include "Complex.h"
#include "Compute.h"
#include <string>
#include <regex>

using namespace std;

Complex::Complex() : real_(0), imag_(0) {}
Complex::Complex(const int r) : real_(r) {}
Complex::Complex(const long long r) : real_(r) {}
Complex::Complex(const long long r, const long long i) : real_(r), imag_(i) {}
Complex::Complex(const char* input) : Complex() { this->set(input); }
Complex::Complex(const std::string& input) : Complex() { this->set(input); }
Complex::Complex(const Integer& r) : real_(r) {}
Complex::Complex(const Decimal& r) : real_(r) {}
Complex::Complex(const Decimal& r, const Decimal& i) : real_(r), imag_(i) {}
Complex::Complex(const Complex& c) : real_(c.real_), imag_(c.imag_) {}

Complex& Complex::operator=(const Complex& c)
{
    if(this == &c) return *this;
    this->real_ = c.real_;
    this->imag_ = c.imag_;
    return *this;
}

Complex::operator Integer() const { return Integer(this->real_); }
Complex::operator Decimal() const { return Decimal(this->real_); }

//const static regex complex("^(i)(\\d*(?:\\.\\d*)?[+-]?)?(\\d*(?:\\.\\d*)?[+-]?)"); //size = 4, [1]:i, [2]:complex, [3]:real

namespace
{
    bool checkDecimal(const string& input)
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
            return true;
        }
        else //break in middle, contains invalid character
        {
            return false;
        }
    }
}

//Mutator
void Complex::set(const std::string& input)
{
    if(input.length() == 0) { this->real_ = 0, this->imag_ = 0; return; }
    
    Complex ret;
    bool err = 0, sign = 0;
    string::const_iterator it = input.begin();
    if(*it == '+' || *it == '-') {
        if(*it == '-') sign = 1;
        it++;
    }
    while(it != input.end() && *it != '+' && *it != '-' && *it != 'i') ++it;
    if(checkDecimal(string(input.begin(), it)))
    { //if suceed
        if(it != input.end() && *it == 'i')
        {
            if(it + 1 == input.end())
            {
                ret.real_ = 0;
				string imagString = string(input.begin(), it);
				ret.imag_ = Decimal(string(input.begin(), it));
				if (ret.imag_ == 0 && (imagString.length() == 0 || (imagString.length() == 1 && (imagString == "-" || imagString == "+"))))
				{
					ret.imag_ = 1;
					if (sign) ret.imag_.setSign(1);
				}
                *this = ret;
                return;
            }
            err = 1;
        }
        else ret.real_ = Decimal(string(input.begin(), it));
    } 
    else err = 1;
    if(!err)
    {
        if(it == input.end())
        {
            ret.imag_ = 0;
            *this = ret;
            return;
        }
        sign = 0;
        if(*it == '+' || *it == '-') {
            if(*it == '-') sign = 1;
        }
        if(*(input.end() - 1) == 'i' && checkDecimal(string(it, input.end() - 1)))
        {
			string imagString = string(it, input.end() - 1);
			ret.imag_ = Decimal(imagString);
			if (ret.imag_ == 0 && (imagString.length() == 0 || (imagString.length() == 1 && (imagString == "-" || imagString == "+"))))
			{
				ret.imag_ = 1;
				if (sign) ret.imag_.setSign(1);
			}
            *this = ret;
            return;
        }

    }
	try {
		*this = (Complex)Compute().solve(input);
	}
	catch (exception &e)
	{
		*this = 0;
	}
}

//Basic Arithmetic
const Complex operator+(const Complex &lhs, const Complex &rhs)
{
    return Complex(lhs.real_ + rhs.real_, lhs.imag_ + rhs.imag_);
}

const Complex operator-(const Complex &lhs, const Complex &rhs)
{
    return Complex(lhs.real_ - rhs.real_, lhs.imag_ - rhs.imag_);
}

//(a+bi)(c+di) = ac-db + (ad+bc)i
const Complex operator*(const Complex &lhs, const Complex &rhs)
{
    return Complex(lhs.real_ * rhs.real_ - lhs.imag_ * rhs.imag_, lhs.real_ * rhs.imag_ + lhs.imag_ * rhs.real_);
}

//(a+bi)/(c+di) = (ac+bd + (bc-ad)i)/(c2 + d2)
const Complex operator/(const Complex &lhs, const Complex &rhs)
{
    Decimal divider = rhs.real_ * rhs.real_ + rhs.imag_ * rhs.imag_; //c2+d2;
    return Complex((lhs.real_ * rhs.real_ + lhs.imag_ * rhs.imag_)/divider, (lhs.imag_ * rhs.real_ - lhs.real_ * rhs.imag_)/divider);
}

const Complex operator%(const Complex &lhs, const Complex &rhs)
{
    if(!lhs.hasComplex() && !rhs.hasComplex()) return Complex(Decimal(lhs) % Decimal(rhs));
    throw runtime_error("MathError: complex number doesn't support mod");
}

//Incremental & Decremental
const Complex& Complex::operator++()
{
    *this += 1;
    return *this;
}
const Complex Complex::operator++(int)
{
    Complex ret(*this);
    ++(*this);
    return ret;
}
const Complex& Complex::operator--()
{
    *this -= 1;
    return *this;    
}
const Complex Complex::operator--(int)
{
    Complex ret(*this);
    --(*this);
    return ret;    
}
const Complex Complex::operator-() const 
{
    Complex ret(*this);
    ret.real_.setSign(!ret.real_.isNeg());
    ret.imag_.setSign(!ret.imag_.isNeg());
    return ret;
}

//Comparator
bool operator==(const Complex &lhs, const Complex &rhs) { return lhs.real_ == rhs.real_ && lhs.imag_ == rhs.imag_; }
bool operator<(const Complex &lhs, const Complex &rhs) { return lhs.real_ < rhs.real_ || (lhs.real_ == rhs.real_ && lhs.imag_ < rhs.imag_); }
bool operator>(const Complex &lhs, const Complex &rhs) { return lhs.real_ > rhs.real_ || (lhs.real_ == rhs.real_ && lhs.imag_ > rhs.imag_); }
bool operator!=(const Complex &lhs, const Complex &rhs) { return !(lhs == rhs); }
bool operator<=(const Complex &lhs, const Complex &rhs) { return (lhs < rhs || lhs == rhs); }
bool operator>=(const Complex &lhs, const Complex &rhs) { return (lhs > rhs || lhs == rhs); }


//Checker
bool Complex::hasComplex() const
{
    return (this->imag_ != 0);
}


//IO
const std::string Complex::str() const
{
    string ret;
    ret += this->real_.str(10);
    if(!this->imag_.isNeg())  ret += '+';
    ret += this->imag_.str(10) + 'i';
    return ret;
}
 
//Read a string and then process it as string.
// there must not be space among the input
void Complex::read(std::istream& is)
{
	string buf;
	is >> buf;
	*this = Complex(buf);
}

void Complex::print(std::ostream& os) const { os<<this->str(); }