//
//  interpreter.cpp
//  ParserImproved
//
//  Created by Katie Rose on 2/4/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//

#include "interpreter.hpp"
#include "parser.hpp"
#include "catch.hpp"

Value *interpret(Expression* inputExpression) {
    
    return inputExpression->evaluate();
}

Expression* optimize(Expression* inputExpression) {
    
    if (inputExpression->containsVariables()) {
        
        return inputExpression->simplify();
    } else {
        
        return inputExpression->evaluate()->toExpression();
    }
}
