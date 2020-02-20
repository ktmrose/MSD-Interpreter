//
//  value.cpp
//  ParserImproved
//
//  Created by Katie Rose on 2/6/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//
#include "expression.hpp"
#include "value.hpp"

NumericValue::NumericValue(int integer) {
    
    this->value = integer;
}

bool NumericValue::equals(Value* value) {
    
    NumericValue* otherNumericValue = dynamic_cast<NumericValue*>(value);
    if (otherNumericValue == nullptr) {
        
        return false;
    } else {
        return (this->value == otherNumericValue->value);
    }
}

Value* NumericValue::addTo(Value* value) {
    
    NumericValue* otherNumericValue = dynamic_cast<NumericValue*>(value);
    if (otherNumericValue == nullptr) {
        
        throw runtime_error("not a number");
    } else {
        return new NumericValue(this->value + otherNumericValue->value);
    }
}

Value* NumericValue::multiplyWith(Value* value) {
    
    NumericValue* otherNumericValue = dynamic_cast<NumericValue*>(value);
    if (otherNumericValue == nullptr) {
        
        throw runtime_error("not a number");
    } else {
        return new NumericValue(this->value * otherNumericValue->value);
    }
}

Expression* NumericValue::toExpression() {
    
    return new Number(this->value);
}

string NumericValue::toString() {
    
    return to_string(this->value);
}

BoolValue::BoolValue(bool conditional) {
    
    this->value = conditional;
}

bool BoolValue::equals(Value* value) {
    
    BoolValue* otherBoolValue = dynamic_cast<BoolValue*>(value);
    if (otherBoolValue == nullptr) {
        
        return false;
    } else {
        return (this->value == otherBoolValue->value);
    }
}

Value* BoolValue::addTo(Value* value) {
    
    throw runtime_error("adding of booleans not supported");
}

Value* BoolValue::multiplyWith(Value* value) {
 
    throw runtime_error("multiplication of booleans not supported");
}

Expression* BoolValue::toExpression() {
    
    return new BoolExpression(this->value);
}

string BoolValue::toString() {
    
    if (value) {
        
        return "_true";
    } else {
        
        return "_false";
    }
}
