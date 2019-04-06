#pragma once

#include "NumberObject.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <set>

class Compute
{
    enum OP
    {
        NUL = 0,
        VAL = 1,
        MINUS = 2,
        //Basic arithmetic operator 10 - 15
        ADD = 10,
        SUB = 11,
        MUL = 12,
        DIV = 13,
        RMD = 14,
        EXP = 15,
        FACT = 16,
        
        //Marks starts at 30
        PAREN_L = 30,
        PAREN_R = 31,
        COMMA = 33,
        //Function starts at >= 100
        SQRT = 100,
        POW = 101
    };
    
    enum Assoc
    {
        LEFT = 0,
        RIGHT = 1
    };

    struct Token
    {
        OP op_;
        std::shared_ptr<NumberObject> no_;
    };
        
    //Mapping from string to their respectively Operator Enum
    const static std::map<std::string, OP> MapOP; 
    const static std::map<OP, std::string> MapOPName; 
    static std::pair<std::map<std::string, Compute::OP> , std::map<Compute::OP, std::string> > initMapOP();
    
    
    std::string input_; //input given by user
    size_t needle_; //current location of where input pointer is.
    
    //Variable System.
    std::map<std::string, std::pair<std::shared_ptr<NumberObject>, NumberObject::ValueType> > variableMap;
    const static std::set<std::string> reservedKeyword;
    void checkVarName(const std::string& varName) ;
    void setVar(const std::string&, const std::shared_ptr<NumberObject>&, NumberObject::ValueType = NumberObject::NONE);
    std::shared_ptr<NumberObject> getVar(const std::string&);
    
    //Build RPN;
    std::queue<Token> RPN; 
    std::stack<OP> OPS;
    Token lastToken;
    
    const OP getOpid(const std::string&);
    const std::string& getOpName(const OP&);
    const Assoc getAssoc(OP op);
    const int getPrecedence(OP op);
    const bool isUnary(const OP&); //check if current token is unary token using lastToken
    const Token getToken(); //Get Tokens from input
    void pushStack(const Token&); //Push Token into building RPN.
    void popStackIntoOutput();
    void buildRPN();
    
    
    bool processSetter(std::string varName, std::string input);
    bool processComparator(std::string comparator, std::string lhs_, std::string rhs_);
public:
    Compute();
    Compute(const std::string&);
    void setInput(const std::string&);
    const std::string operator()();
    const std::string operator()(const std::string&);
    std::shared_ptr<NumberObject> solve();
    std::shared_ptr<NumberObject> solve(const std::string&);
};