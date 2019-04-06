#include "NumberObject.h"
#include "Integer.h"
#include "Decimal.h"
#include "Complex.h"
#include "Tools.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

using namespace std;
typedef NumberObject::ValueType ValueType;

istream& operator>>(istream& is, NumberObject& obj)
{
    obj.read(is);
    return is;
}

ostream& operator<<(ostream& os, const NumberObject& obj) 
{
    obj.print(os);
    return os;
}

const string NumberObject::TypeName(ValueType id)
{
    switch(id)
    {
        case NumberObject::NONE: return "NumberObject";
        case NumberObject::INTEGER: return "Integer";
        case NumberObject::DECIMAL: return "Decimal";
        case NumberObject::COMPLEX: return "Complex";
        case NumberObject::POWEROBJ: return "PowerObject";
        default: return string("Unsupported Type with id: ") + to_string(id);
    }
}

const shared_ptr<NumberObject> pow(const shared_ptr<const NumberObject>&lhs , const shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(pow(Integer(lhs), Integer(rhs))));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(pow(Decimal(lhs), Decimal(rhs))));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(pow(Complex(lhs), Complex(rhs))));
        default: throw runtime_error(string("MathError: unsupported type for pow. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

const shared_ptr<NumberObject> sqrt(const shared_ptr<const NumberObject>&lhs)
{
    ValueType TargetType = lhs->TypeID();
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(sqrt(Integer(lhs))));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(sqrt(Decimal(lhs))));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(sqrt(Complex(lhs))));
        default: throw runtime_error(string("MathError: unsupported type for sqrt. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

const shared_ptr<NumberObject> fact(const shared_ptr<const NumberObject>&lhs)
{
    ValueType TargetType = lhs->TypeID();
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(fact(Integer(lhs))));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(fact(Decimal(lhs))));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(fact(Complex(lhs))));
        default: throw runtime_error(string("MathError: factorial only valid for integer"));
    }
}


const shared_ptr<NumberObject> operator+(const shared_ptr<const NumberObject>&lhs, const shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(Integer(lhs) + Integer(rhs)));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(Decimal(lhs) + Decimal(rhs)));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(Complex(lhs) + Complex(rhs)));
        default: throw runtime_error(string("MathError: unsupported type for +. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

const shared_ptr<NumberObject> operator-(const shared_ptr<const NumberObject>&lhs, const shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(Integer(lhs) - Integer(rhs)));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(Decimal(lhs) - Decimal(rhs)));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(Complex(lhs) - Complex(rhs)));
        default: throw runtime_error(string("MathError: unsupported type for -. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

const shared_ptr<NumberObject> operator*(const shared_ptr<const NumberObject>&lhs, const shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(Integer(lhs) * Integer(rhs)));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(Decimal(lhs) * Decimal(rhs)));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(Complex(lhs) * Complex(rhs)));
        default: throw runtime_error(string("MathError: unsupported type for *. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

const shared_ptr<NumberObject> operator/(const shared_ptr<const NumberObject>&lhs, const shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(Integer(lhs) / Integer(rhs)));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(Decimal(lhs) / Decimal(rhs)));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(Complex(lhs) / Complex(rhs)));
        default: throw runtime_error(string("MathError: unsupported type for /. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

const shared_ptr<NumberObject> operator%(const shared_ptr<const NumberObject>&lhs, const shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return shared_ptr<Integer>(new Integer(Integer(lhs) % Integer(rhs)));
        case NumberObject::DECIMAL: return shared_ptr<Decimal>(new Decimal(Decimal(lhs) % Decimal(rhs)));
        case NumberObject::COMPLEX: return shared_ptr<Complex>(new Complex(Complex(lhs) % Complex(rhs)));
        default: throw runtime_error(string("MathError: unsupported type for %. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

const shared_ptr<NumberObject> operator+=(shared_ptr<NumberObject>& lhs, const shared_ptr<const NumberObject>& rhs) {return (lhs = (lhs + rhs));}
const shared_ptr<NumberObject> operator-=(shared_ptr<NumberObject>& lhs, const shared_ptr<const NumberObject>& rhs) {return (lhs = (lhs - rhs));}
const shared_ptr<NumberObject> operator*=(shared_ptr<NumberObject>& lhs, const shared_ptr<const NumberObject>& rhs) {return (lhs = (lhs * rhs));}
const shared_ptr<NumberObject> operator/=(shared_ptr<NumberObject>& lhs, const shared_ptr<const NumberObject>& rhs) {return (lhs = (lhs / rhs));;}
const shared_ptr<NumberObject> operator%=(shared_ptr<NumberObject>& lhs, const shared_ptr<const NumberObject>& rhs) {return (lhs = (lhs % rhs));}


bool o_equal_to(const std::shared_ptr<const NumberObject>&lhs, const std::shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return Integer(lhs) == Integer(rhs);
        case NumberObject::DECIMAL: return Decimal(lhs) == Decimal(rhs);
        case NumberObject::COMPLEX: return Complex(lhs) == Complex(rhs);
        default: throw runtime_error(string("MathError: unsupported type for ==. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

bool o_less(const std::shared_ptr<const NumberObject>&lhs, const std::shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return Integer(lhs) < Integer(rhs);
        case NumberObject::DECIMAL: return Decimal(lhs) < Decimal(rhs);
        case NumberObject::COMPLEX: return Complex(lhs) < Complex(rhs);
        default: throw runtime_error(string("MathError: unsupported type for <. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

bool o_greater(const std::shared_ptr<const NumberObject>&lhs, const std::shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return Integer(lhs) > Integer(rhs);
        case NumberObject::DECIMAL: return Decimal(lhs) > Decimal(rhs);
        case NumberObject::COMPLEX: return Complex(lhs) > Complex(rhs);
        default: throw runtime_error(string("MathError: unsupported type for >. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

bool o_not_equal_to(const std::shared_ptr<const NumberObject>&lhs, const std::shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return Integer(lhs) != Integer(rhs);
        case NumberObject::DECIMAL: return Decimal(lhs) != Decimal(rhs);
        case NumberObject::COMPLEX: return Complex(lhs) != Complex(rhs);
        default: throw runtime_error(string("MathError: unsupported type for !=. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

bool o_less_equal(const std::shared_ptr<const NumberObject>&lhs, const std::shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return Integer(lhs) <= Integer(rhs);
        case NumberObject::DECIMAL: return Decimal(lhs) <= Decimal(rhs);
        case NumberObject::COMPLEX: return Complex(lhs) <= Complex(rhs);
        default: throw runtime_error(string("MathError: unsupported type for <=. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}

bool o_greater_equal(const std::shared_ptr<const NumberObject>&lhs, const std::shared_ptr<const NumberObject>&rhs)
{
    ValueType TargetType = max(lhs->TypeID(), rhs->TypeID());
    switch(TargetType)
    {
        case NumberObject::INTEGER: return Integer(lhs) >= Integer(rhs);
        case NumberObject::DECIMAL: return Decimal(lhs) >= Decimal(rhs);
        case NumberObject::COMPLEX: return Complex(lhs) >= Complex(rhs);
        default: throw runtime_error(string("MathError: unsupported type for >=. type: '") + NumberObject::TypeName(TargetType) + "'");
    }
}
    

//Constructors

Integer::Integer(const shared_ptr<const NumberObject>& obj)
{
    if(obj->TypeID() == 1) *this = static_cast<const Integer&>(*obj);
    else if(obj->TypeID() == 2) *this = Integer(static_cast<const Decimal&>(*obj));
    else if(obj->TypeID() == 3) *this = Integer(static_cast<const Complex&>(*obj));
}

Decimal::Decimal(const shared_ptr<const NumberObject>& obj)
{
    if(obj->TypeID() == 1) *this = static_cast<const Integer&>(*obj);
    else if(obj->TypeID() == 2) *this = static_cast<const Decimal&>(*obj);
    else if(obj->TypeID() == 3) *this = Decimal(static_cast<const Complex&>(*obj));
}

Complex::Complex(const shared_ptr<const NumberObject>& obj)
{
    if(obj->TypeID() == 1) *this = static_cast<const Integer&>(*obj);
    else if(obj->TypeID() == 2) *this = static_cast<const Decimal&>(*obj);
    else if(obj->TypeID() == 3) *this = static_cast<const Complex&>(*obj);
}