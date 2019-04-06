#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <cmath>

class NumberObject
{
public:
    enum ValueType
    {
        NONE = 0,
        INTEGER = 1,
        DECIMAL = 2,
        COMPLEX = 3,
        POWEROBJ = 4,
        FREE = 10
    };
    NumberObject(){};
    virtual const ValueType TypeID() const { return NONE; }
    const static std::string TypeName(ValueType);
    virtual const std::string str() const = 0;
    virtual void read(std::istream& is) = 0;
    virtual void print(std::ostream& os) const = 0;
    friend std::istream& operator>>(std::istream&, NumberObject&);
    friend std::ostream& operator<<(std::ostream&, const NumberObject&);
    
private:
    friend const std::shared_ptr<NumberObject> pow(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> sqrt(const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> fact(const std::shared_ptr<const NumberObject>&);

    friend const std::shared_ptr<NumberObject> operator+(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator-(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator*(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator/(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator%(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    
    friend const std::shared_ptr<NumberObject> operator+=(std::shared_ptr<NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator-=(std::shared_ptr<NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator*=(std::shared_ptr<NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator/=(std::shared_ptr<NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend const std::shared_ptr<NumberObject> operator%=(std::shared_ptr<NumberObject>&, const std::shared_ptr<const NumberObject>&);
    
    friend bool o_equal_to(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend bool o_less(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend bool o_greater(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend bool o_not_equal_to(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend bool o_less_equal(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    friend bool o_greater_equal(const std::shared_ptr<const NumberObject>&, const std::shared_ptr<const NumberObject>&);
    
};
