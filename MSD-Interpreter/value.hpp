//
//  value.hpp
//  ParserImproved
//
//  Created by Katie Rose on 2/6/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//

#ifndef value_hpp
#define value_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Expression;

/*
 Value is the most simplified version of an Expression.
 */
class Value {
public:
    
    virtual bool equals(Value* value) = 0;
    virtual Value* addTo(Value* otherValue) = 0;
    virtual Value* multiplyWith(Value* otherVal) = 0;
    virtual Expression* toExpression() = 0;
    virtual string toString() = 0;
};

class NumericValue : public Value {
    
public:
    int value;
    NumericValue(int integer);
    bool equals(Value* value) override;
    Value* addTo(Value* otherValue) override;
    Value* multiplyWith(Value* otherValue) override;
    Expression * toExpression() override;
    string toString() override;
};

class BoolValue : public Value {
    
public:
  bool value;
  BoolValue(bool conditional);
  bool equals(Value* value) override;
  Value* addTo(Value* otherValue) override;
  Value* multiplyWith(Value* otherValue) override;
  Expression* toExpression() override;
  string toString() override;
};

#endif /* value_hpp */
