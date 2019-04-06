#include "Compute.h"
#include "Integer.h"
#include "Decimal.h"
#include "Complex.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <regex>
#include <memory>
#include <stdexcept>
#include <set>

using namespace std;
typedef NumberObject::ValueType ValueType;


pair<map<string, Compute::OP>, map<Compute::OP, string> > Compute::initMapOP()
{
    map<string, Compute::OP> m;
    map<Compute::OP, string> n;
    m["+"] = ADD, n[ADD] = "+";
    m["-"] = SUB, n[SUB] = "-";
    m["*"] = MUL, n[MUL] = "*";
    m["/"] = DIV, n[DIV] = "/";
    m["%"] = RMD, n[RMD] = "%";
    m["^"] = EXP, n[EXP] = "^";
    m["!"] = FACT, n[FACT] = "!";
    m["("] = PAREN_L, n[PAREN_L] = "(";
    m[")"] = PAREN_R, n[PAREN_R] = ")";
    m["sqrt"] = SQRT, n[SQRT] = "sqrt";
    m["pow"] = POW, m["power"] = POW, n[POW] = "pow";
    m[","] = COMMA, n[COMMA] = ",";
    return pair<map<string, Compute::OP> , map<Compute::OP, string> >(m, n);
}

const map<string, Compute::OP> Compute::MapOP(initMapOP().first);
const map<Compute::OP, string> Compute::MapOPName(initMapOP().second);

const set<string> Compute::reservedKeyword = {"set"};

const Compute::OP Compute::getOpid(const string& s)
{
    string query(s);
    transform(query.begin(), query.end(), query.begin(), ::tolower);
    map<string, Compute::OP>::const_iterator it = MapOP.find(query);
    if(it != MapOP.end()) return it->second;
    return NUL;
}

const string& Compute::getOpName(const Compute::OP& query)
{
    map<Compute::OP, string>::const_iterator it = MapOPName.find(query);
    if(it != MapOPName.end()) return it->second;
    throw runtime_error(string("InternalError: undefined operator: ") + to_string(query));
}

const Compute::Assoc Compute::getAssoc(OP op){
    switch(op){
        case ADD: case SUB: case MUL: case DIV: case RMD: case FACT: case PAREN_R: 
            return LEFT;
        case EXP: case SQRT: case POW: case MINUS: 
            return RIGHT;
            
        default: throw runtime_error(string("InternalError: undefined assiciativity. operator: ") + getOpName(op));
    }
}

const int Compute::getPrecedence(OP op){
    switch(op){
        case ADD: 
        case SUB: return 2;
        case MUL:
        case DIV: 
        case RMD: return 3;
        case EXP: return 4;
        case SQRT: return 10;
        case POW:
        case FACT:
        case MINUS: return 100;
        case PAREN_L: return 0;
        
        default: throw runtime_error(string("InternalError: undefined precedence. operator: ") + getOpName(op));
    }
    
}

//check if current token is unary token using lastToken
const bool Compute::isUnary(const Compute::OP& op)
{
    switch(op){
        case ADD: case SUB: case MUL: case DIV: case RMD: case EXP: case COMMA: case PAREN_L: case NUL: case MINUS:
            return true;
            
        default: return false;
    }
}

const Compute::Token Compute::getToken()
{
    if(this->needle_ >= this->input_.size())
    {
        throw runtime_error("InternalError: Token Out of Range!");
    }
    //Regex just for value matching. matching from complex > decimal > integer, accordingly
    const static regex complex("^(\\d*(?:\\.\\d*)?i(?![a-zA-Z0-9]))"); //only match the complex part, 'i' may be a leading char of variable/function.
    const static regex decimal("^(\\d*?\\.\\d*)"); 
    const static regex integer("^(\\d+)"); 
    const static regex unary("^([+-]+)");
    const static regex operators("^([+-/%*^!()])");
    const static regex function("^([a-zA-Z]+[a-zA-Z0-9]*)"); //Variable | Function name must start with alphabet character.
    smatch sm;
    OP op(VAL);
    shared_ptr<NumberObject> no(nullptr);
    //cout<<input_<<endl;
    if(regex_search(input_.cbegin() + needle_, input_.cend(), sm, complex))
    {
        no = make_shared<Complex>(sm[1]);
    }
    else if(regex_search(input_.cbegin() + needle_, input_.cend(), sm, decimal))
    {
        no = make_shared<Decimal>(sm[1]);
    }
    else if(regex_search(input_.cbegin() + needle_, input_.cend(), sm, integer))
    {
        no = make_shared<Integer>(sm[1]);
    }
    else if(isUnary(lastToken.op_) && regex_search(input_.cbegin() + needle_, input_.cend(), sm, unary))
    {
        //cout<<"TEST"<<endl;
        this->needle_ += sm[0].length();
        int countNeg = 0; string query(sm[1]);
        for(char &c : query) if(c == '-') ++countNeg;
        if(countNeg % 2 == 1) return Token{MINUS};
        else return Token{NUL};
    }
    else if(regex_search(input_.cbegin() + needle_, input_.cend(), sm, operators))
    {
        op = getOpid(sm[1]);
    }
    else if(regex_search(input_.cbegin() + needle_, input_.cend(), sm, function))
    {
        op = getOpid(sm[1]);
        if(op == NUL) //maybe it is a variableName, check for variable
        {
            op = VAL;
            no = getVar(sm[1]); //getVar will throw a exception if no variable are found.
        }
    }
    else if(isspace(*(input_.cbegin() + needle_))) //clear spaces.
    {
        while(needle_ < input_.size() && isspace(*(input_.cbegin() + needle_))) needle_++;
        return Token{NUL};
    }
    else
    {
        string buffer; 
        string::iterator it = input_.begin() + needle_;
        for(int i = 0; i < 10 && it != input_.cend(); ++i, ++it)
        {
            buffer.push_back(*it);
        }
        throw runtime_error(string("NameError: token \'") + buffer + string("\'... is invalid."));
    }
    this->needle_ += sm[0].length();
    //if(op == VAL) cout<<no<<"     oooooo     "<<*no<<endl;
    return Token{op, no};
}


void Compute::popStackIntoOutput()
{
    if(OPS.size() > 0)
    {
        RPN.push(Token{OPS.top()});
        OPS.pop();
    }
    else throw runtime_error(string("InternalError: please contact admin. 0x01"));
}

void Compute::pushStack(const Compute::Token &token)
{
    const OP &op = token.op_;
    //if(token.op_ == VAL) cout<<token.no_<<"     mmmmmm     "<<*token.no_<<endl;
    switch(op)
    { 
        case VAL: 
            if(lastToken.op_ == PAREN_R) pushStack(Token{MUL});
            RPN.push(token); 
            break;

        case COMMA: case PAREN_R:
        
            while(OPS.size() > 0 && OPS.top() != PAREN_L)
            {
                popStackIntoOutput();
            }
            if(op == PAREN_R)
            {
                if(OPS.size() == 0) throw runtime_error("SyntaxError: mismatched parentheses.");
                else 
                {
                    OPS.pop(); //Pop the PAREN_L
                }
            }
            
            break;
        //Arithmetic
        case ADD: case SUB: case MUL: case DIV: case RMD: case EXP: case FACT: case MINUS:
        
            if(OPS.size() > 0)
            {
                Assoc assoc = getAssoc(op);
                int precedence = getPrecedence(op), precedence2;
                while(OPS.size() > 0)
                {
                    precedence2 = getPrecedence(OPS.top());
                    if((assoc == LEFT && precedence <= precedence2)
                    || (assoc == RIGHT && precedence < precedence2)) popStackIntoOutput();
                    else break;
                }
            }
            OPS.push(op);
            break;
        
        
        //Function
        case PAREN_L: case SQRT:  case POW:
        
            if(lastToken.op_ == PAREN_R || lastToken.op_ == VAL) pushStack(Token{MUL});
            OPS.push(op);
            break;
        //Undefined Behaviour
        default:
        throw runtime_error(string("InternalError: please contact admin. 0x02"));
    }
}

void Compute::buildRPN()
{
    needle_ = 0;
    RPN = queue<Token>();
    OPS = stack<OP>();
    Token token = lastToken = Token{NUL};
    if(input_.empty())
    {
        RPN.push(Token{VAL, make_shared<Integer>(0)});
        return;
    }
    while(needle_ < input_.size())
    {
        if(token.op_ != NUL) lastToken = token;
        token = getToken();
        if(token.op_ == NUL) continue;
        pushStack(token);
    }
    //Finalize
    while(!OPS.empty())
    {
        RPN.push(Token{OPS.top()});
        OPS.pop();
    }
    
    /*while(!RPN.empty())
    {
        cout<<RPN.front().op_<<"      aaaaaa      "<<RPN.front().no_;
        if(RPN.front().op_ == VAL) cout<<"         "<<*RPN.front().no_;
        cout<<endl;
        RPN.pop();
    }*/
}

std::shared_ptr<NumberObject> Compute::solve()
{
    buildRPN();
    Token token;
    stack<shared_ptr<NumberObject>> numberPile;
    vector<shared_ptr<NumberObject>> buffer;
    shared_ptr<NumberObject> ans;
    
    while(!RPN.empty())
    {
        token = RPN.front(), RPN.pop();
        const OP &op = token.op_;
        buffer.clear();
        ans = nullptr;
        if(op == PAREN_L) continue;
        switch(op)
        {
            case VAL:
                ans = token.no_;
                break;
                
            //unary operands
            case MINUS: case SQRT: case FACT:
            
                //check amount of operands first.
                if(numberPile.empty()) throw runtime_error("SyntaxError: no operand.");
                
                buffer.push_back(numberPile.top()), numberPile.pop();
                
                switch(op)
                {
                    case MINUS: ans = buffer[0] * make_shared<Integer>(-1); break;
                    case SQRT: ans = sqrt(buffer[0]); break;      
                    case FACT: ans = fact(buffer[0]); break;
                    default:;
                }
                break;
                
            //case of 2 operands.
            case ADD: case SUB: case MUL: case DIV: case EXP: case RMD: case POW:
            
                //check amount of operands first.
                if(numberPile.size() < 2)  throw runtime_error(string("SyntaxError: mismatched operand. operator: ") + getOpName(op)); 
                
                //pop operand into buffer
                for(int i = 0; i < 2; ++i) buffer.push_back(numberPile.top()), numberPile.pop();
                
                switch(op)
                {
                    case ADD: ans = buffer[1] + buffer[0]; break;
                    case SUB: ans = buffer[1] - buffer[0]; break;
                    case MUL: ans = buffer[1] * buffer[0]; break;
                    case DIV: ans = buffer[1] / buffer[0]; break;
                    case RMD: ans = buffer[1] % buffer[0]; break;
                    case EXP: ans = pow(buffer[1], buffer[0]); break;
                    case POW: ans = pow(buffer[1], buffer[0]); break;
                    default:;
                }
                break;
            
            default:; //undefined operator , ans = nullptr
        }
        if(ans == nullptr) throw runtime_error(string("InternalError: operator \'") + getOpName(op) + string("\'... hasn't been implemented yet."));
        numberPile.push(ans);
    }
    if(numberPile.empty())
    {
        throw runtime_error("SyntaxError: no operand.");
    }
    else if(numberPile.size() > 1)
    {
        throw runtime_error("SyntaxError: too many values.");
    }
    return numberPile.top();
}

std::shared_ptr<NumberObject> Compute::solve(const std::string& input_)
{ 
    this->setInput(input_); 
    return this->solve();
}

Compute::Compute(){}

Compute::Compute(const string& input) { this->setInput(input); }

void Compute::setInput(const string& input)
{
    this->input_ = input;
}

void Compute::checkVarName(const string& varName)
{
    if(varName.size() == 0) throw runtime_error("NameError: variable name can't be empty");
    else if(varName.size() >= 1 && !isalpha(varName[0])) throw runtime_error("NameError: variable name must starts with alphabet.");
    for(const char &c : varName)
    {
        if(!isalnum(c)) throw runtime_error("NameError: variable name must be composed of alphanumeric characters (leading character must be alphabet).");
    }
    
    if(varName == "i") throw runtime_error("NameError: imaginary number 'i' can't be a variable name.");
    
    string lowerVarName(varName);
    transform(lowerVarName.begin(), lowerVarName.end(), lowerVarName.begin(), ::tolower);
    
    map<string, Compute::OP>::const_iterator itOP = MapOP.find(lowerVarName);
    set<string>::const_iterator itReserved = reservedKeyword.find(lowerVarName);
    
    if(itOP == MapOP.end() && itReserved == reservedKeyword.end()) return;     
    else throw runtime_error("NameError: name '" + varName + "' is reserved keyword."); //var name should not be operator name;
}


void Compute::setVar(const string& name, const shared_ptr<NumberObject>& var, ValueType type)
{
    checkVarName(name); //throw exception if name doesn't match requirement.
    
    typedef pair<shared_ptr<NumberObject>, ValueType> value_pair; //value pair
    switch(type)
    {
        //Update variable
        case NumberObject::NONE:
            {
                map<string, value_pair >::iterator it = variableMap.find(name);
                if(it == variableMap.end())
                {
                    throw runtime_error(string("NameError: failed to update variable. variable \'") + name + ("\' doesn't exist."));
                }
                else
                {
                    setVar(name, var, (it->second).second);
                }
            }
            break;
        //Set variable
        case NumberObject::INTEGER: 
            variableMap[name] = value_pair (make_shared<Integer>(var), NumberObject::INTEGER);
            break;
        case NumberObject::DECIMAL: 
            variableMap[name] = value_pair (make_shared<Decimal>(var), NumberObject::DECIMAL);
            break;
        case NumberObject::COMPLEX: 
            variableMap[name] = value_pair (make_shared<Complex>(var), NumberObject::COMPLEX);
            break;
        case NumberObject::FREE:
            variableMap[name] = value_pair (var, NumberObject::FREE);
            break;
        case NumberObject::POWEROBJ:
            break;
    
    }
}

shared_ptr<NumberObject> Compute::getVar(const string& query)
{
    map<string, pair<shared_ptr<NumberObject>, ValueType> >::iterator it = variableMap.find(query);
    if(it == variableMap.end())
    {
        throw runtime_error(string("NameError: variable \'") + query + ("\' doesn't exist."));
    }
    return variableMap[query].first;
}

//If has setter, process it.
bool Compute::processSetter(string setter, string input)
{
    const static regex setReg("^\\s*[Ss][Ee][Tt]\\s+(\\S*)\\s+(\\S*)\\s*$");
    const static regex varReg("^\\s*(\\w+)\\s*$");
    smatch sm;
    ValueType type = NumberObject::NONE;
    string varName;
    if(regex_match(setter.cbegin(), setter.cend(), sm, setReg)) 
    {
        string typeString = sm[1];
        transform(typeString.begin(), typeString.end(), typeString.begin(), ::tolower);
        
        if(typeString == "integer") type = NumberObject::INTEGER;
        else if(typeString == "decimal") type = NumberObject::DECIMAL;
        else if(typeString == "complex") type = NumberObject::COMPLEX;
        else if(typeString == "free") type = NumberObject::FREE;
        else throw runtime_error(string("SyntaxError: invalid type: '") + string(sm[1]) + ("', valid types are 'integer', 'decimal', 'complex', 'free'."));
        
        varName = sm[2];
    }
    else if(regex_match(setter.cbegin(), setter.cend(), sm, varReg))
    {
        varName = sm[1];
    }
    else throw runtime_error("SyntaxError: you meant to be using 'set <type> <name> = <value>'?");
    
    setVar(varName, this->solve(input), type);
    return true;
}

bool Compute::processComparator(string comparator, string lhs_, string rhs_)
{
    const shared_ptr<NumberObject> lhs = this->solve(lhs_);
    const shared_ptr<NumberObject> rhs = this->solve(rhs_);
    if(comparator == "==") return o_equal_to(lhs, rhs);
    else if(comparator == "<") return o_less(lhs, rhs);
    else if(comparator == ">") return o_greater(lhs, rhs);
    else if(comparator == "!=") return o_not_equal_to(lhs, rhs);
    else if(comparator == "<=") return o_less_equal(lhs, rhs);
    else if(comparator == ">=") return o_greater_equal(lhs, rhs);
    
    throw runtime_error(string("SyntaxError: unrecognized operator: ") + comparator);
}

const string Compute::operator()()
{
    const static regex comparator("^(.*)([!<>=]=|[<>])(.*)");
    const static regex setter("^(.*)=(.*)|^[Ss][Ee][Tt](.*)");
    smatch sm;
    try
    {
        //if the operation is to compare
        if(regex_match(this->input_.cbegin(), this->input_.cend(), sm, comparator)) 
        {
            if(processComparator(sm[2], sm[1], sm[3])) return "True";
            else return "False";
        }
        //if the operation is to set variable.
        else if(regex_match(this->input_.cbegin(), this->input_.cend(), sm, setter)) 
        {
            if(processSetter(sm[1], sm[2])) return "ok!";
            else throw runtime_error("InternalError: setter error!");
        }
        else
        {
        	return this->solve()->str();   
        }
    }
    catch(exception& e)
    {
        return e.what();
    }
}

const string Compute::operator()(const string& input)
{
    this->setInput(input); 
    return this->operator()();
}