//
//  expression.hpp
//  ParserImproved
//
//  Created by Katie Rose on 2/2/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//

#ifndef expression_hpp
#define expression_hpp

#include <stdio.h>
#include <string>
#include "value.hpp"

using namespace std;


/*
 Expression is an abstract class.  Things that are considered Expressions are Numbers, Variables, and any combination of Numbers and Variables seperated by arithmetic operators.
 */
class Expression {
public:
    
    virtual bool equals(Expression *expr) = 0;
    virtual Value* evaluate() = 0;
    virtual bool containsVariables() = 0;
    virtual Expression* substitute(string variable, Value* value) = 0;
    virtual Expression* simplify() = 0;
    virtual string toString() = 0;
};

/*
 Number is a positive integer
*/
class Number : public Expression {
public:
    
    int value;
    Number(int inputValue);
    bool equals(Expression *expr) override;
    Value* evaluate() override;
    bool containsVariables() override;
    Expression* substitute(string variable, Value* value) override;
    Expression* simplify() override;
    string toString() override;
};

/*
 Variable is any combination of alphabetic characters
 */
class Variable : public Expression {
public:
    
    string name;
    Variable (string name);
    bool equals(Expression *expr) override;
    Value* evaluate() override;
    bool containsVariables() override;
    Expression* substitute(string variable, Value* value) override;
    Expression* simplify() override;
    string toString() override;
};

/*
 Add is responsible for coordinating the left hand side and right hand sife of a '+'
 */
class Add : public Expression {
public:
    
    Expression *leftHandSide;
    Expression *rightHandSide;
    
    Add(Expression *lhs, Expression *rhs);
    bool equals(Expression *expr) override;
    Value* evaluate() override;
    bool containsVariables() override;
    Expression* substitute(string variable, Value* value) override;
    Expression* simplify() override;
    string toString() override;
};

/*
 Same as above, but for '*'
 */
class Multiply : public Expression {
public:
    
    Expression *leftHandSide;
    Expression *rightHandSide;
       
    Multiply(Expression *lhs, Expression *rhs);
    bool equals(Expression *expr) override;
    Value* evaluate() override;
    bool containsVariables() override;
    Expression* substitute(string variable, Value* value) override;
    Expression* simplify() override;
    string toString() override;
};

class BoolExpression : public Expression {
public:
    bool boolean;
    
    BoolExpression(bool conditional);
    bool equals(Expression *expr) override;
    Value* evaluate() override;
    bool containsVariables() override;
    Expression * substitute(string variable, Value *value) override;
    Expression* simplify() override;
    string toString() override;
};

class LetExpression : public Expression {
public:
    Variable* subVariable;
    Expression* subExpression;
    Expression* subBody;
    
    LetExpression(Variable* substituteVariable, Expression* substituteExpression, Expression* substituteBody);
    bool equals(Expression *expr) override;
    Value* evaluate() override;
    bool containsVariables() override;
    Expression* substitute(string variable, Value* value) override;
    Expression* simplify() override;
    string toString() override;
};

#endif
