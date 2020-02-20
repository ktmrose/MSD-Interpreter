//
//  parser.cpp
//  ParserImproved
//
//  Created by Katie Rose on 2/2/20.
//  Copyright © 2020 MSD Katie Rose. All rights reserved.
//
#include <iostream>
#include <sstream>
#include "parser.hpp"
#include "catch.hpp"

using namespace std;

static Expression *parseExpression(istream &in);
static Expression *parseAddend(istream &in);
static Expression *parseInner(istream &in);
static Expression *parseNumber(istream &in);
static Variable *parseVariable(istream &in);
static string parseAlphabetic(istream &input, string prefix);
static string parseKeyword(istream &input);
static char peekAfterSpaces(istream &in);

/*
 Take an input stream that contains an expression, and returns the parsed representation of that expression. Throws `runtime_error` for parse errors.
 */
Expression *parse(istream &input) {
    
    Expression *expr = parseExpression(input);
    char inputCharacter = peekAfterSpaces(input);
    if (!input.eof()) {
        
        throw runtime_error((string)"expected end of file at " + inputCharacter);
    }
    return expr;
}

/*
 Takes an input stream that starts with an expression, consuming the largest initial expression possible.
 */
static Expression *parseExpression(istream &input) {
    
    Expression *expr = parseAddend(input);
    char inputCharacter = peekAfterSpaces(input);
    if (inputCharacter == '+') {
        
        input >> inputCharacter;
        Expression *rightHandSide = parseExpression(input);
        expr = new Add(expr, rightHandSide);
    }
    return expr;
}

/*
 Takes an input stream that starts with an addend, consuming the largest initial addend possible, where an addend is an expression that does not have `+` except within nested expressions (like parentheses).
 */
static Expression *parseAddend(istream &input) {
    
    Expression *expr = parseInner(input);
    char inputCharacter = peekAfterSpaces(input);
    if (inputCharacter == '*') {
        
        inputCharacter = input.get();
        Expression *rightHandSide = parseAddend(input);
        expr = new Multiply(expr, rightHandSide);
    }
    return expr;
}

/*
 Parses something with no immediate `+` or `*` from `in`
 */
static Expression *parseInner(istream &input) {
    
    Expression *expr;
    char inputCharacter = peekAfterSpaces(input);
    if (inputCharacter == '(') {
        
        inputCharacter = input.get();
        expr = parseExpression(input);
        inputCharacter = peekAfterSpaces(input);
        if (inputCharacter == ')') {
            
            inputCharacter = input.get();
        } else {
            
            throw runtime_error("expected a close parenthesis");
        }
        
    } else if (isdigit(inputCharacter)) {
        
        expr = parseNumber(input);
    } else if (isalpha(inputCharacter)) {
        
        expr = parseVariable(input);
        
    } else if (inputCharacter == '_') {
        
        string keyword = parseKeyword(input);
                if (keyword == "_true") {
        
                    return new BoolExpression(true);
                } else if (keyword == "_false") {
        
                    return new BoolExpression(false);
                    
                } else if (keyword == "_let") { //let x = 5 in x + 9 outputs 14
                    
                    //get variable name (maybe this should be a string instead?)
                    peekAfterSpaces(input);
                    Variable* subVariable = parseVariable(input);
                    Expression* subExpression;
                    Expression* subBody;
            
                    if (peekAfterSpaces(input) == '=') {
                        
                        input.get();
                        subExpression = parseExpression(input);
                    } else {
                        
                        throw runtime_error((string)"expected '=' after variable substitution");
                    }
                    keyword = parseKeyword(input);
                    if (keyword == "_in") {
                        
                        subBody = parseExpression(input);
                        return (new LetExpression(subVariable, subExpression, subBody));
                        
                    } else {
                        
                        throw runtime_error((string)"expected keyword _in after _let substitution");
                    }
                } else {
        
                    throw std::runtime_error((std::string)"unexpected keyword " + keyword);
                }
    } else {
        
        throw std::runtime_error((std::string)"expected a digit or open parenthesis at " + inputCharacter);
    }
    
    return expr;
}

static string parseAlphabetic(istream &input, string prefix) {
    
    string name = prefix;
    while (1) {
        char inputCharacter = input.peek();
        if (!isalpha(inputCharacter)) {
            break;
        }
        name += input.get();
    }
    return name;
}

static string parseKeyword(istream &input) {
    input.get();
    return parseAlphabetic(input, "_");
}

// Parses a number, assuming that `in` starts with a digit.
static Expression *parseNumber(istream &input) {
    int num = 0;
    input >> num;
    return new Number(num);
}

/*
 Parses an expression, assuming that `in` starts with a letter.
 */
static Variable *parseVariable(istream &input) {
    
    return new Variable(parseAlphabetic(input, ""));
}

/*
 Like in.peek(), but consume an whitespace at the start of `in`
 */
static char peekAfterSpaces(istream &input) {
    char inputCharacter;
    while (1) {
        
        inputCharacter = input.peek();
        if (!isspace(inputCharacter)) {
            
            break;
        }
        inputCharacter = input.get();
    }
    return inputCharacter;
}

/* for tests */
static Expression *parse_str(string s) {
    std::istringstream in(s);
    return parse(in);
}

/* for tests */
static std::string parse_str_error(std::string s) {
    std::istringstream in(s);
    try {
        (void)parse(in);
        return "";
    } catch (std::runtime_error exn) {
        return exn.what();
    }
}

TEST_CASE( "See if this works" ) {
    Expression *ten_plus_one = new Add(new Number(10), new Number(1));
    
    CHECK ( parse_str_error(" ( 1 ") == "expected a close parenthesis" );
    
    CHECK( parse_str("10")->equals(new Number(10)) );
    CHECK( parse_str("(10)")->equals(new Number(10)) );
    CHECK( parse_str("10+1")->equals(ten_plus_one) );
    CHECK( parse_str("(10+1)")->equals(ten_plus_one) );
    CHECK( parse_str("(10)+1")->equals(ten_plus_one) );
    CHECK( parse_str("10+(1)")->equals(ten_plus_one) );
    CHECK( parse_str("1+2*3")->equals(new Add(new Number(1),
                                              new Multiply(new Number(2), new Number(3)))) );
    CHECK( parse_str("1*2+3")->equals(new Add(new Multiply(new Number(1), new Number(2)),
                                              new Number(3))) );
    CHECK( parse_str("4*2*3")->equals(new Multiply(new Number(4),
                                                   new Multiply(new Number(2), new Number(3)))) );
    CHECK( parse_str("4+2+3")->equals(new Add(new Number(4),
                                              new Add(new Number(2), new Number(3)))) );
    CHECK( parse_str("4*(2+3)")->equals(new Multiply(new Number(4),
                                                     new Add(new Number(2), new Number(3)))) );
    CHECK( parse_str("(2+3)*4")->equals(new Multiply(new Add(new Number(2), new Number(3)),
                                                     new Number(4))) );
    CHECK( parse_str("xyz")->equals(new Variable("xyz")) );
    CHECK( parse_str("xyz+1")->equals(new Add(new Variable("xyz"), new Number(1))) );
    
    CHECK ( parse_str_error("!") == "expected a digit or open parenthesis at !" );
    CHECK ( parse_str_error("(1") == "expected a close parenthesis" );
    
    CHECK( parse_str(" 10 ")->equals(new Number(10)) );
    CHECK( parse_str(" (  10 ) ")->equals(new Number(10)) );
    CHECK( parse_str(" 10  + 1")->equals(ten_plus_one) );
    CHECK( parse_str(" ( 10 + 1 ) ")->equals(ten_plus_one) );
    CHECK( parse_str(" 11 * ( 10 + 1 ) ")->equals(new Multiply(new Number(11),
                                                               ten_plus_one)) );
    CHECK( parse_str(" ( 11 * 10 ) + 1 ")
          ->equals(new Add(new Multiply(new Number(11), new Number(10)),
                           new Number (1))) );
    CHECK( parse_str(" 1 + 2 * 3 ")
          ->equals(new Add(new Number(1),
                           new Multiply(new Number(2), new Number(3)))) );
    
    CHECK ( parse_str_error(" ! ") == "expected a digit or open parenthesis at !" );
    CHECK ( parse_str_error(" ( 1 ") == "expected a close parenthesis" );
    CHECK ( parse_str_error(" 1 )") == "expected end of file at )" );
}

TEST_CASE( "Boolean support test ") {
    CHECK( parse_str( " _false ")->equals(new BoolExpression(false)) );
    CHECK( parse_str( " _true ")->equals(new BoolExpression(true)) );
}

TEST_CASE( "Let Expression support test" ) {
//    CHECK( parse_str("_let x = 5 _in x + 2")->equals(new Number(7)));
}
