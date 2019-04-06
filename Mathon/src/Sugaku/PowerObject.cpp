#include "NumberObject.h"
#include "PowerObject.h"
#include "Integer.h"
#include "Decimal.h"
#include "Complex.h"
#include <iostream>
#include <memory>
#include <string>
#include <list>

using namespace std;

//
// (val_ + C_ * sqrt(powerset[0] + powerset[1] +...)) / den_
//

//Default Constructor
PowerObject::PowerObject() 
: val_(make_shared<Integer>(0)), exp_(make_shared<Integer>(1)), C_(make_shared<Integer>(1)) {}

//Integer Constructor
PowerObject::PowerObject(const Integer& val) 
: val_(make_shared<Integer>(val)), exp_(make_shared<Integer>(1)), C_(make_shared<Integer>(1)) {}

//Decimal Constructor
PowerObject::PowerObject(const Decimal& val)
: val_(make_shared<Decimal>(val)), exp_(make_shared<Integer>(1)), C_(make_shared<Integer>(1)) {}

//Complex Constructor
PowerObject::PowerObject(const Complex& val)  
: val_(make_shared<Complex>(val)), exp_(make_shared<Integer>(1)), C_(make_shared<Integer>(1)) {}

//Power Constructor
PowerObject::PowerObject(const shared_ptr<NumberObject>& base, const shared_ptr<NumberObject>& exp)
: val_(base), exp_(exp),  C_(make_shared<Integer>(1)) {}

PowerObject::PowerObject(const shared_ptr<NumberObject>& base, const shared_ptr<NumberObject>& exp, const shared_ptr<NumberObject>& C)
: val_(base), exp_(exp),  C_(make_shared<Integer>(C)) {}

void PowerObject::insertPowerobj(const PowerObject &obj)
{
    list<PowerObject>::iterator it = this->powerset_.begin();
    while(it != this->powerset_.end() && o_less(obj.val_, it->val_))
    {
        it++;
    }
    if(it == this->powerset_.end()) this->powerset_.insert(it, obj);
    else if(o_equal_to(obj.val_, it->val_) && o_equal_to(obj.exp_, it->exp_)){
        it->C_ += obj.C_;
        if(o_equal_to(it->C_, make_shared<Integer>(0))) this->powerset_.erase(it);
    }
    else this->powerset_.insert(it, obj);
}

const PowerObject operator+(const PowerObject &lhs, const PowerObject &rhs)
{
    PowerObject ret;
    if(o_equal_to(lhs.exp_, make_shared<Integer>(1)) && o_equal_to(ret.exp_, rhs.exp_))
    {
        ret.val_ = lhs.val_ + rhs.val_;
        ret.powerset_ = lhs.powerset_;
        list<PowerObject>::const_iterator it_rhs = rhs.powerset_.begin();
        while(it_rhs != rhs.powerset_.end())
        {
            ret.insertPowerobj(*it_rhs);
            it_rhs++;
        }
    }
    else {
        ret.insertPowerobj(lhs);
        ret.insertPowerobj(rhs);
    }
    return ret;
}

const PowerObject operator-(const PowerObject &lhs, const PowerObject &rhs_)
{
    PowerObject rhs(rhs_);
    rhs.C_ *= make_shared<Integer>(-1);
    return (lhs + rhs);
}

const PowerObject operator*(const PowerObject &lhs, const PowerObject &rhs)
{
    PowerObject ret(lhs);
    if(o_equal_to(ret.exp_, make_shared<Integer>(1)) && o_equal_to(ret.exp_, rhs.exp_)) ret.val_ += rhs.val_;
    else ret.insertPowerobj(rhs);
    list<PowerObject>::const_iterator it_rhs = rhs.powerset_.begin();
    while(it_rhs != rhs.powerset_.end())
    {
        ret.insertPowerobj(*it_rhs);
    }
    return ret;
}

const PowerObject operator/(const PowerObject &lhs, const PowerObject &rhs)
{
    PowerObject ret(lhs);
    if(o_equal_to(ret.exp_, make_shared<Integer>(1)) && o_equal_to(ret.exp_, rhs.exp_)) ret.val_ += rhs.val_;
    else ret.insertPowerobj(rhs);
    list<PowerObject>::const_iterator it_rhs = rhs.powerset_.begin();
    while(it_rhs != rhs.powerset_.end())
    {
        ret.insertPowerobj(*it_rhs);
    }
    return ret;
}

bool operator==(const PowerObject &lhs, const PowerObject &rhs) 
{ 
    if(o_not_equal_to(lhs.val_, rhs.val_) 
    || o_not_equal_to(lhs.exp_, rhs.exp_) 
    || o_not_equal_to(lhs.C_, rhs.C_)  
    || lhs.powerset_.size() != rhs.powerset_.size()) return false;
    
    list<PowerObject>::const_iterator it_lhs = lhs.powerset_.begin(), it_rhs = rhs.powerset_.begin();
    while(it_lhs != lhs.powerset_.end())
    {
        if(!(*it_lhs == *it_rhs)) return false;
        it_lhs++;
        it_rhs++;
    }
    return true;
}

bool operator!=(const PowerObject &lhs, const PowerObject &rhs) 
{ 
    return !(lhs == rhs);
}

const std::string PowerObject::str() const
{
    string ret;
    if(o_not_equal_to(C_, make_shared<Integer>(1))) ret.append(C_->str());
    ret.push_back('(');
    ret.append(val_->str());
    for(const PowerObject &obj : powerset_)
    {
        ret.push_back('+');
        ret.append(obj.str());
    }
    ret.push_back(')');
    ret.push_back('^');
    ret.append(exp_->str());
    return ret;
}
void PowerObject::read(std::istream& is)
{
}
void PowerObject::print(std::ostream& os) const
{
    os<<this->str()<<endl;
}