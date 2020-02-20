//
//  main.cpp
//  ParserImproved
//
//  Created by Katie Rose on 2/2/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//

#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "expression.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    Catch::Session().run(argc, argv);

    Expression* e = parse(cin);
    Value* output = interpret(e);

    cout << output->toString() + "\n";
    return 0;
}
