//
//  expression.cpp
//  ParserImproved
//
//  Created by Katie Rose on 2/2/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//

#include "expression.hpp"
#include "catch.hpp"
#include "value.hpp"

Number::Number(int val) {
    
    this->value = val;
}

bool Number::equals(Expression *expr) {
    
    Number *num = dynamic_cast<Number*>(expr);
    if (num == nullptr) {
        
        return false;
    } else {
        
        return value == num->value;
    }
}

Value* Number::evaluate() {

    return new NumericValue(this->value);
}

bool Number::containsVariables() {
    return false;
}

Expression* Number::substitute(string variable, Value* value) {
    return new Number(this->value);
}

Expression* Number::simplify() {
    
    if (containsVariables()) {
        throw runtime_error("Cannot simplify further");
    }
    
    return this;
}

Add::Add(Expression *lhs, Expression *rhs) {
    
    this->leftHandSide = lhs;
    this->rightHandSide = rhs;
}

bool Add::equals(Expression *expr) {
    
    Add *add = dynamic_cast<Add*>(expr);
    if (add == nullptr) {
        
        return false;
    } else {
        
        return (leftHandSide->equals(add->leftHandSide)
                && rightHandSide->equals(add->rightHandSide));
    }
}

Value* Add::evaluate() {
    
    Value* lhs = this->leftHandSide->evaluate();
    Value* rhs = this->rightHandSide->evaluate();
    if (lhs == nullptr || rhs == nullptr) {
        
        throw runtime_error((string)"Current version only supports addition of integers");
    }
    
    return lhs->addTo(rhs);
}

bool Add::containsVariables() {
    
    return (leftHandSide->containsVariables() || rightHandSide->containsVariables());
}

Expression* Add::substitute(string variable, Value* value) {
    
    return new Add(leftHandSide->substitute(variable, value), rightHandSide->substitute(variable, value));
}

Expression* Add::simplify() {
    
    Expression* lhs = leftHandSide->simplify();
    Expression* rhs = rightHandSide->simplify();
    
    //add check to see if lhs and rhs are Numbers.  if they are numbers, you can combine them
    Number* numlhs = dynamic_cast<Number*>(lhs);
    Number* numrhs = dynamic_cast<Number*>(rhs);
    if (numlhs != nullptr && numrhs != nullptr) {
        
        return new Number(numrhs->value + numlhs->value);
    }
    
    return new Add(lhs, rhs);
}

Multiply::Multiply (Expression *lhs, Expression *rhs) {
    
    this->leftHandSide = lhs;
    this->rightHandSide = rhs;
}

bool Multiply::equals(Expression *expr) {
    
    Multiply *multiply = dynamic_cast<Multiply*>(expr);
    if (multiply == nullptr) {
        
        return false;
    } else {
        
        return (leftHandSide->equals(multiply->leftHandSide)
                && rightHandSide->equals(multiply->rightHandSide));
    }
}

Value* Multiply::evaluate() {
    Value* lhs = this->leftHandSide->evaluate();
    Value* rhs = this->rightHandSide->evaluate();
    
    if (lhs == nullptr || rhs == nullptr) {
        
        throw runtime_error((string)"Current version only supports multiplication of integers");
    }
    
    return lhs->multiplyWith(rhs);
}

bool Multiply::containsVariables() {
    
    return (leftHandSide->containsVariables() || rightHandSide->containsVariables());
}

Expression* Multiply::substitute(string variable, Value* value) {
    
    return new Multiply(leftHandSide->substitute(variable, value), rightHandSide->substitute(variable, value));
}

Expression* Multiply::simplify() {
    
    Expression* lhs = leftHandSide->simplify();
    Expression* rhs = rightHandSide->simplify();
    Number* numlhs = dynamic_cast<Number*>(lhs);
    Number* numrhs = dynamic_cast<Number*>(rhs);
    if (numlhs != nullptr && numrhs != nullptr) {
        
        return new Number(numrhs->value * numlhs->value);
    }
    
    return new Multiply(lhs, rhs);
}

Variable::Variable(string inputName) {
    
    this->name = inputName;
}

bool Variable::equals(Expression *expr) {
    
    Variable *value = dynamic_cast<Variable*>(expr);
    if (value == nullptr) {
        
        return false;
    } else {
        
        return name == value->name;
    }
}

Value* Variable::evaluate() {
    
    throw runtime_error((string)"Incomplete substitution");
}

bool Variable::containsVariables() {
    
    return true;
}

Expression* Variable::substitute(string variable, Value* value) {
    
    if (variable == this->name) {
        return value->toExpression();
    } else {
        return this;
    }
}

Expression* Variable::simplify() {
    
    return this;
}

BoolExpression::BoolExpression(bool conditional) {

    this->boolean = conditional;
}

bool BoolExpression::equals(Expression* expr) {
    
    BoolExpression* b = dynamic_cast<BoolExpression*>(expr);
    if (b == NULL)
      return false;
    else
    return (this->boolean == b->boolean);
}

Value* BoolExpression::evaluate() {
    
    return new BoolValue(this->boolean);
}

bool BoolExpression::containsVariables() {
    
    return false;
}

Expression* BoolExpression::substitute(string variable, Value* value) {
    
    return this;
}

Expression* BoolExpression::simplify() {
    
    return this;
}

LetExpression::LetExpression(Variable* substituteVariable, Expression* substituteValue, Expression* substituteBody) {
    
    this->subVariable = substituteVariable;
    this->subExpression = substituteValue;
    this->subBody = substituteBody;
}

bool LetExpression::equals(Expression *expr) {
    
    LetExpression* letExpr = dynamic_cast<LetExpression*>(expr);
    if (letExpr == NULL)
      return false;
    else
    return (this->subVariable->equals(letExpr->subVariable) && this->subExpression->equals(letExpr->subExpression) && this->subBody->equals(letExpr->subBody));
}

Value* LetExpression::evaluate() {
    
    Expression* newExpression = subBody->substitute(subVariable->name, subExpression->evaluate());
    return newExpression->evaluate();
}

bool LetExpression::containsVariables() {
    
    return true;
}

Expression* LetExpression::substitute(string variable, Value* value) {
    
    return new LetExpression(new Variable(variable), subExpression->substitute(variable, value), subBody->substitute(variable, value));
}

Expression* LetExpression::simplify() {
    
    return new LetExpression(subVariable, subExpression->simplify(), subBody->simplify());
}

TEST_CASE( "equals" ) {
    
  CHECK( (new Number(1))->equals(new Number(1)) );
  CHECK( ! (new Number(1))->equals(new Number(2)) );
  CHECK( ! (new Number(1))->equals(new Multiply(new Number(2), new Number(4))) );
  CHECK( (new Variable("x"))->equals(new Variable("x")) );
}

TEST_CASE( "evaluate" ) {
    
    CHECK( (new Multiply(new Number(6), new Number(4)) )->evaluate()->equals(new NumericValue(24)) ) ;
    CHECK( (new Add(new Number(6), new Number(4)) )->evaluate()->equals(new NumericValue(10)) );
    CHECK( (new Add(new Number(2), (new Multiply(new Number(6), new Number(4) )) ))->evaluate()->equals(new NumericValue(26)));
    CHECK( (new LetExpression(new Variable("x"), new Number(5), (new Add (new Variable("x"), new Number(11)))))->evaluate()->equals(new NumericValue(16)));
}

TEST_CASE( "contains Variables" ) {
    
    CHECK( (new Number(90))->containsVariables() == false );
    CHECK( (new Variable("toot"))->containsVariables() == true);
    CHECK( (new Add (new Number(12), new Number(11)))->containsVariables() == false );
    CHECK( (new Multiply (new Number(12), new Number(11)))->containsVariables() == false );
    CHECK( (new Add (new Variable("you"), new Number(11)))->containsVariables() == true );
    CHECK( (new Multiply (new Number(12), new Variable("frog")))->containsVariables() == true );
}

TEST_CASE( "substitution" ) {
    
    CHECK( (new Variable("toad"))->substitute("toad", (new NumericValue(3)))->equals(new Number(3)));
    CHECK( (new Variable("squib"))->substitute("toad", (new NumericValue(3)))->equals(new Variable("squib")) );
    CHECK( (new Variable("toad"))->substitute("toad", (new BoolValue(true)))->equals(new BoolExpression(true)));
    CHECK( (new Variable("squib"))->substitute("toad", (new BoolValue(false)))->equals(new Variable("squib")));
    CHECK( (new LetExpression(new Variable("x"), new Number(5), (new Add (new Variable("x"), new Number(11)))))->substitute("x", new NumericValue(5))->equals((new LetExpression(new Variable("x"), new Number(5), (new Add (new Number(5), new Number(11)))))) );
    CHECK( ((new LetExpression(new Variable("y"), new Number(5), (new Add (new Variable("x"), new Number(11)))))->substitute("y", new NumericValue(5)))->equals(new LetExpression(new Variable("y"), new Number(5), (new Add (new Variable("x"), new Number(11))))) );
}

TEST_CASE( "simplify" ) {
    
    //checking base case
    CHECK( (new Number(25))->simplify()->equals(new Number(25)));
    CHECK( (new Variable("test"))->simplify()->equals(new Variable("test")));
    
    //checking more complicated objects
    CHECK( (new Multiply(new Number(6), new Number(4)) )->simplify()->equals(new Number(24)) ) ;
    CHECK( (new Add(new Number(6), new Number(4)) )->simplify()->equals(new Number(10)) );
    CHECK( (new Add(new Number(2), (new Multiply(new Number(6), new Number(4) )) ))->simplify()->equals(new Number(26)));
    
    //checking even more complicated objects
    CHECK( (new Multiply(new Number(6), new Variable("x")) )->simplify()->equals(new Multiply(new Number(6), new Variable("x")) ) ) ;
    CHECK( (new Add(new Variable("test"), new Number(4)) )->simplify()->equals(new Add(new Variable("test"), new Number(4)) ));
    CHECK( (new Add(new Variable("one"), (new Multiply(new Number(6), new Number(4) )) ))->simplify()->equals(new Add(new Variable("one"), new Number(24))) );
    
    //check LetExpressions
//    CHECK( (new LetExpression(new Variable("x"), new Number(5), (new Add (new Variable("x"), new Number(11)))))->simplify()->equals((new LetExpression(new Variable("x"), new Number(5), (new Add (new Number(5), new Number(11)))))) );
    
}
