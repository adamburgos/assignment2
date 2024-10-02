#ifndef _booleanEvaluation_h
#define _booleanEvaluation_h

#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "booleanWithError.h"
#include "tokenizeString.h"

void printName( void );
char *evaluatePostfix( char *str );
char *postfixToInfix( char *str );
int isBinaryOperator(char *token);
boolean evaluateBinaryOperator(char *operator, boolean op1, boolean op2);

#endif
