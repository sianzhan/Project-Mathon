#pragma once

#include "NumberObject.h"
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <memory>

class Integer : public NumberObject
{
private:
    std::vector<int> limb_;
    bool sign_;
    
    Integer(const Integer& z, bool sign); //Copy Constructor with Signness
    Integer(const std::vector<int>&, bool = 0); //Constructor with vector
    
    //At operator overload
    int& operator[](int i) { return this->limb_[i]; }
    const int& operator[](int i) const { return this->limb_[i]; }
    
    //Checker Function
    const bool isNumber(const std::string&) const;
    const size_t size() const;
    
    void clear() {this->limb_.clear(), this->sign_ = 0; }
    void clearLeadingZero();
    void complement(const size_t& size);
    

public:
    virtual const NumberObject::ValueType TypeID() const override { return NumberObject::INTEGER; }
    Integer();
    Integer(const std::string&);
    Integer(const int); 
    Integer(const long long); 
    Integer(const double);
    
    Integer(const std::shared_ptr<const NumberObject>&);
    Integer(const Integer&); //Copy Constructor
    Integer(const char*);
    Integer& operator=(const Integer&); //Copy Assignment
    Integer& operator=(const char*); //Copy Assignment
    
    operator std::shared_ptr<NumberObject>();
    
    //Mutator Function
    void set(long long); //Set using Compute;
    void set(const std::string&);
    
    void setSign(bool);
    
    friend const Integer operator+(const Integer &lhs, const Integer &rhs);
    friend const Integer operator-(const Integer &lhs, const Integer &rhs);
    friend const Integer operator*(const Integer &lhs, const Integer &rhs);
    friend const Integer operator/(const Integer &lhs, const Integer &rhs);
    friend const Integer operator%(const Integer &lhs, const Integer &rhs);
    
    friend const std::pair<Integer, Integer> fmod(const Integer &lhs, const Integer &rhs); //Used in operator/ & operator% for calculation
    
    const Integer& operator+=(const Integer &rhs) { return *this = *this + rhs; }
    const Integer& operator-=(const Integer &rhs) { return *this = *this - rhs; }
    const Integer& operator*=(const Integer &rhs) { return *this = *this * rhs; }
    const Integer& operator/=(const Integer &rhs) { return *this = *this / rhs; }
    const Integer& operator%=(const Integer &rhs) { return *this = *this % rhs; }
    
    const Integer& operator++();
    const Integer operator++(int);
    const Integer& operator--();
    const Integer operator--(int);
    
    const Integer operator-() const { return Integer(*this, !this->isNeg()); }
    
    //Comparator
    friend bool operator==(const Integer &lhs, const Integer &rhs);
    friend bool operator<(const Integer &lhs, const Integer &rhs);
    friend bool operator>(const Integer &lhs, const Integer &rhs);
    friend bool operator!=(const Integer &lhs, const Integer &rhs);
    friend bool operator<=(const Integer &lhs, const Integer &rhs);
    friend bool operator>=(const Integer &lhs, const Integer &rhs);
    
    //Checker
    const bool isZero() const;
    const bool isNeg() const;
    const bool isEven() const;
    const bool isOdd() const;
    
    const Integer& shift(int digit);
    const std::string str() const override;
    void read(std::istream& is) override;
    void print(std::ostream& os) const override { os<<this->str(); }

};