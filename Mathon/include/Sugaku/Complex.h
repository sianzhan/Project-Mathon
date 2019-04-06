#pragma once

#include "NumberObject.h"
#include "Integer.h"
#include "Decimal.h"
#include <vector>
#include <iostream>
#include <string>


class Complex : public NumberObject
{
private:
    Decimal real_; 
    Decimal imag_;
public:
    virtual const NumberObject::ValueType TypeID() const override { return NumberObject::COMPLEX; }
    Complex();
    Complex(const int);
    Complex(const long long);
    Complex(const long long, const long long);
    Complex(const char*);
    
    Complex(const std::string&);
    Complex(const Integer&);
    Complex(const Decimal&);
    Complex(const Decimal&, const Decimal&);
    Complex(const Complex&);
    Complex(const std::shared_ptr<const NumberObject>&);
    
    Complex& operator=(const Complex&);
    
    explicit operator Integer() const;
    explicit operator Decimal() const;
    
    void set(const std::string&);
    const Decimal real() { return this->real_; }
    const Decimal imag() { return this->imag_; }
    
    friend const Complex operator+(const Complex &lhs, const Complex &rhs);
    friend const Complex operator-(const Complex &lhs, const Complex &rhs);
    friend const Complex operator*(const Complex &lhs, const Complex &rhs);
    friend const Complex operator/(const Complex &lhs, const Complex &rhs);
    friend const Complex operator%(const Complex &lhs, const Complex &rhs);
    
    const Complex& operator+=(const Complex &rhs) { return *this = *this + rhs; }
    const Complex& operator-=(const Complex &rhs) { return *this = *this - rhs; }
    const Complex& operator*=(const Complex &rhs) { return *this = *this * rhs; }
    const Complex& operator/=(const Complex &rhs) { return *this = *this / rhs; }
    const Complex& operator%=(const Complex &rhs) { return *this = *this % rhs; }
    
    const Complex& operator++();
    const Complex operator++(int);
    const Complex& operator--();
    const Complex operator--(int);
    const Complex operator-() const;
    
    friend bool operator==(const Complex &lhs, const Complex &rhs);
    friend bool operator<(const Complex &lhs, const Complex &rhs);
    friend bool operator>(const Complex &lhs, const Complex &rhs);
    friend bool operator!=(const Complex &lhs, const Complex &rhs);
    friend bool operator<=(const Complex &lhs, const Complex &rhs);
    friend bool operator>=(const Complex &lhs, const Complex &rhs);
    
    //Checker
    bool hasComplex() const; //check if has complex part
    
    const std::string str() const override;
    void read(std::istream& is) override;
    void print(std::ostream& os) const override;
};