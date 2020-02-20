//
//  interpreter.hpp
//  ParserImproved
//
//  Created by Katie Rose on 2/4/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//

#ifndef interpreter_hpp
#define interpreter_hpp
#include "expression.hpp"

#include <stdio.h>

Value *interpret(Expression* parsedExpression);

Expression* optimize(Expression* inputExpression);
#endif /* interpreter_hpp */
