#pragma once

#include "NumberObject.h"
#include "Integer.h"
#include "Decimal.h"
#include "Complex.h"
#include <iostream>
#include <list>
#include <memory>
#include <string>

class PowerObject : public NumberObject
{
    std::shared_ptr<NumberObject> val_;
    std::shared_ptr<NumberObject> exp_;
    std::shared_ptr<NumberObject> C_;
    std::list<PowerObject>powerset_;
    
public:
    virtual const NumberObject::ValueType TypeID() const override { return NumberObject::POWEROBJ; }
    PowerObject();
    PowerObject(const Integer&);
    PowerObject(const Decimal&);
    PowerObject(const Complex&);
    PowerObject(const std::shared_ptr<NumberObject>& base, const std::shared_ptr<NumberObject>& exp);
    PowerObject(const std::shared_ptr<NumberObject>& base, const std::shared_ptr<NumberObject>& exp, const std::shared_ptr<NumberObject>& C);
    
    void insertPowerobj(const PowerObject &obj);
    friend const PowerObject operator+(const PowerObject&, const PowerObject&);
    friend const PowerObject operator-(const PowerObject&, const PowerObject&);
    friend const PowerObject operator*(const PowerObject&, const PowerObject&);
    friend const PowerObject operator/(const PowerObject&, const PowerObject&);
    
    friend bool operator==(const PowerObject &lhs, const PowerObject &rhs);
    friend bool operator<(const PowerObject &lhs, const PowerObject &rhs);
    friend bool operator>(const PowerObject &lhs, const PowerObject &rhs);
    friend bool operator!=(const PowerObject &lhs, const PowerObject &rhs);
    friend bool operator<=(const PowerObject &lhs, const PowerObject &rhs);
    friend bool operator>=(const PowerObject &lhs, const PowerObject &rhs);
    
    virtual const std::string str() const override;
    virtual void read(std::istream& is) override;
    virtual void print(std::ostream& os) const override;
    
    friend const PowerObject sqrt(const PowerObject &base);
};