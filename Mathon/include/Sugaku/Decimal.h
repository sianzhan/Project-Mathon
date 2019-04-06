#pragma once

#include <vector>
#include <iostream>
#include <string>

class NumberObject;
class Integer;

class Decimal : public NumberObject
{
private:
    Integer num_; //Numerator , Sign depends on numerator
    Integer den_;
    
    void clearTrailingZero();
    void simplify();
    void clear() {this->num_ = 0; this->den_ = 1; }
    
public:
    virtual const NumberObject::ValueType TypeID() const override { return NumberObject::DECIMAL; }
    Decimal();
    Decimal(const int);
    Decimal(const long long);
    Decimal(const long long, const long long);
    Decimal(const double);
    Decimal(const char*);
    
    Decimal(const std::string&);
    Decimal(const Integer&);
    Decimal(const Integer&, const Integer&);
    Decimal(const Decimal&);
    Decimal(const std::shared_ptr<const NumberObject>&);
    
    Decimal& operator=(const Decimal&);
    
    explicit operator Integer() const;
    
    void set(const std::string&);
    void setSign(bool);
    
    friend const Decimal operator+(const Decimal &lhs, const Decimal &rhs);
    friend const Decimal operator-(const Decimal &lhs, const Decimal &rhs);
    friend const Decimal operator*(const Decimal &lhs, const Decimal &rhs);
    friend const Decimal operator/(const Decimal &lhs, const Decimal &rhs);
    friend const Decimal operator%(const Decimal &lhs, const Decimal &rhs);
    
    const Decimal& operator+=(const Decimal &rhs) { return *this = *this + rhs; }
    const Decimal& operator-=(const Decimal &rhs) { return *this = *this - rhs; }
    const Decimal& operator*=(const Decimal &rhs) { return *this = *this * rhs; }
    const Decimal& operator/=(const Decimal &rhs) { return *this = *this / rhs; }
    const Decimal& operator%=(const Decimal &rhs) { return *this = *this % rhs; }
    
    const Decimal& operator++();
    const Decimal operator++(int);
    const Decimal& operator--();
    const Decimal operator--(int);
    const Decimal operator-() const { return Decimal(*this * -1); }
    
    friend bool operator==(const Decimal &lhs, const Decimal &rhs);
    friend bool operator<(const Decimal &lhs, const Decimal &rhs);
    friend bool operator>(const Decimal &lhs, const Decimal &rhs);
    friend bool operator!=(const Decimal &lhs, const Decimal &rhs);
    friend bool operator<=(const Decimal &lhs, const Decimal &rhs);
    friend bool operator>=(const Decimal &lhs, const Decimal &rhs);
    
    const bool isNeg() const { return (this->num_).isNeg(); }
    const bool isInteger() const;
    
    const std::string str() const override {return this->str(100);}
    const std::string str(int precision)  const;
    void read(std::istream& is) override;
    void print(std::ostream& os) const override { os<<this->str(); }
};