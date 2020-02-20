# MSD-Interpreter
Latest functional version of an arithmetic interpreter.  This is an ongoing project that will be built on throughout the semester.

This is a command line tool that can evaluate arithmetic expressions of intergers. Currently supports addition, multiplication, and nested parentheses and evaluates these expressions in proper arithmetic order.

Variables can also be included in expression and their corresponding values can be substituted in the proper place using the syntax:

_let variable = expression _in  expression

so for example:

_let phi = 2 + 3 _in 5 * (x + 10)

Design and code was adapted from the professor's starting point.
