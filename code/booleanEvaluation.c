#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "stack.h"
#include "booleanEvaluation.h"
#include "booleanWithError.h"


/* TODO:
 * printName
 * input: none
 * output: none
 *
 * Prints name of the student who worked on this solution
 */
void printName( void )
{
    /* TODO : Fill in your name */
    printf("This solution was completed by:\n");
    printf("<Adam Burgos>\n");
}

/* TODO:
 * evaluatePostfix
 * input: a postfix expression (calls tokenizeString to get the tokens from the string)
 * output: T, F, or E
 *
 * Uses a stack to evaluates the postfix expression and returns the result as a string where "T" denotes true and "F" denotes false.
 * If the postfix expression is invalid it returns "E" to denote an error has occurred.  Be sure to use the helpful functions in booleanEvaluation.
 *
 * Hints:
 * - DO NOT free the parameter str.  It is freed by the calling function.
 * - Be sure to use the helpful functions in booleanEvaluation.
 * - Test your code often!  Get it working correctly for the first couple test cases instead of trying to solve all of them at once.
 */
char *evaluatePostfix( char *str )
{
    /* Your variable Declarations: */
    int tokenCount, i;
    char** arrTokens = tokenizeString( str, &tokenCount ); 
    boolean result = TRUE;
    Stack* s = createStack();

    if (arrTokens == NULL){
        freeStack(s);
        return booleanToString(ERROR);  
    }

    for (i = 0 ; i < tokenCount; i++){
        char *token = arrTokens[i];

        if (strequals(token, "T") == TRUE || strequals(token, "F") == TRUE){
            push(s, strdup(token));
        }

        else if (strequals(token, "NOT") == TRUE){
            char* op1Str;
            char* resStr;
            boolean op1;
            boolean res;
            
            if (s->top < 0){
                result = ERROR; 
                break; 
            }
            op1Str = pop(s);
            op1 = stringToBoolean(op1Str);
            free(op1Str);

            if (op1 == ERROR){
                result = ERROR;
                break; 
            }

            if (op1 == TRUE){
                res = FALSE;
            } else {
                res = TRUE;
            }

            resStr = booleanToString(res);
            push(s, resStr);
        }
        else if (isBinaryOperator(token)){
            char* op2Str;
            char* op1Str;
            char* resStr;
            boolean op2;
            boolean op1;
            boolean res;

            if (s->top < 1){
                result = ERROR; 
                break;
            }

            op2Str = pop(s);
            op1Str = pop(s);
            op2 = stringToBoolean(op2Str);
            op1 = stringToBoolean(op1Str);
            free(op2Str);
            free(op1Str);

            if (op1 == ERROR || op2 == ERROR){
                result = ERROR;
                break; 
            }

            res = evaluateBinaryOperator(token, op1, op2);
            if (res == ERROR){
                result = ERROR;
                break;
            }

            resStr = booleanToString(res);
            push(s, resStr);
        } 
        else {
            result = ERROR; 
            break;
        }
    }

    if (result == ERROR){
        for (i = 0; i < tokenCount; i++){
            free(arrTokens[i]);
        }
        free(arrTokens);
        freeStack(s);
        return booleanToString(ERROR);
    }   

    if (s->top != 0){
        result = ERROR;
    } else {
        char *finalResult = pop(s);
        result = stringToBoolean(finalResult);
        free(finalResult);
    }

    for (i = 0; i < tokenCount; i++){
        free(arrTokens[i]);
    }
    free(arrTokens);
    freeStack(s);

    return booleanToString( result ); 
}

/* TODO:
 * postfixToInfix
 * input: a postfix expression (calls tokenizeString to get the tokens from the string)
 * output: the equivalent infix expression
 *
 * Uses a stack to convert str to its equivalent expression in infix.
 * You can assume that the postfix expression is valid
 *
 * Hints:
 * - DO NOT free the parameter str.  It is freed by the calling function.
 * - Be sure to use the helpful functions in booleanEvaluation.
 * - Test your code often!  Get it working correctly for the first couple test cases instead of trying to solve all of them at once.
 */
char *postfixToInfix( char *str )
{
    /* Your variable Declarations: */
    int tokenCount, i, j;
    char** arrTokens = tokenizeString( str, &tokenCount );
    Stack* s = createStack();

    if (arrTokens == NULL){
        freeStack(s);
        return NULL;
    }

    for (i = 0; i < tokenCount; i++){
        char* token = arrTokens[i];

        if (strcmp(token, "T") == 0 || strcmp(token, "F") == 0){
            push(s, strdup(token));
        }
        else if (strequals(token, "NOT") == TRUE){
            char* op1Str;
            char* expr;
            int length = 0;

            if (s->top < 0){
                for (i = 0; i < tokenCount; i++){
                    free(arrTokens[i]);
                }
                free(arrTokens);
                freeStack(s);
                return NULL; 
            }

            op1Str = pop(s);

            length = strlen(token) + strlen(op1Str) + 3;
            expr = (char*)malloc((length + 1) * sizeof(char));
            if (expr == NULL) {
                free(op1Str);
                for (j = 0; j < tokenCount; j++){
                    free(arrTokens[j]);
                }
                free(arrTokens);
                freeStack(s);
                return NULL;
            }

            sprintf(expr, "( %s %s )", token, op1Str);

            push(s, expr);

            free(op1Str);
        }
        else if (isBinaryOperator(token)){
            char* op2Str;
            char* op1Str;
            char* expr; 
            int length = 0;

            if (s->top < 1){
                for (j = 0; j < tokenCount; j++){
                    free(arrTokens[j]);
                }
                free(arrTokens);
                freeStack(s);
                return NULL;
            }

            op2Str = pop(s);
            op1Str = pop(s);

            length = strlen(op1Str) + strlen(token) + strlen(op2Str) + 4;
            expr = (char*)malloc((length + 1) * sizeof(char));
            if (expr == NULL){
                free(op1Str);
                free(op2Str);
                for (j = 0; j < tokenCount; j++){
                    free(arrTokens[j]);
                }
                free(arrTokens);
                freeStack(s);
                return NULL;
            }

            sprintf(expr, "( %s %s %s )", op1Str, token, op2Str);

            push(s, expr);

            free(op1Str);
            free(op2Str);
        } else {
            for(j = 0; j < tokenCount; j++){
                free(arrTokens[j]);
            }
            free(arrTokens);
            freeStack(s);
            return NULL;
        }

        free(token);
    
    }

    free(arrTokens);

    if(s->top != 0){
        freeStack(s);
        return NULL;
    } else {
        char* finalResult = pop(s);
        freeStack(s);
        return finalResult;
    }

}

/* You may want to create some helper functions down here! */
int isBinaryOperator(char *token){
    if (strequals(token, "AND")     == TRUE ||
        strequals(token, "OR")      == TRUE ||
        strequals(token, "NAND")    == TRUE ||
        strequals(token, "NOR")     == TRUE ||
        strequals(token, "XOR")     == TRUE ||
        strequals(token, "COND")    == TRUE ||
        strequals(token, "BICOND")  == TRUE){
        return 1; 
    } else {
        return 0; 
    }
}
/* TODO: change this */
boolean evaluateBinaryOperator(char *operator, boolean op1, boolean op2){
    if (strequals(operator, "AND") == TRUE) {
        if (op1 == TRUE && op2 == TRUE) {
            return TRUE;
        } else {
            return FALSE;
        }
    } 
    else if (strequals(operator, "NAND") == TRUE) {
        if (op1 == TRUE && op2 == TRUE) {
            return FALSE;
        } else {
            return TRUE;
        }
    } 
    else if (strequals(operator, "OR") == TRUE) {
        if (op1 == TRUE || op2 == TRUE) {
            return TRUE;
        } else {
            return FALSE;
        }
    } 
    else if (strequals(operator, "NOR") == TRUE) {
        if (op1 == TRUE || op2 == TRUE) {
            return FALSE;
        } else {
            return TRUE;
        }
    } 
    else if (strequals(operator, "XOR") == TRUE) {
        if (op1 != op2) {
            return TRUE;
        } else {
            return FALSE;
        }
    } 
    else if (strequals(operator, "COND") == TRUE) {
        if (op1 == FALSE || op2 == TRUE) {
            return TRUE;
        } else {
            return FALSE;
        }
    } 
    else if (strequals(operator, "BICOND") == TRUE) {
        if (op1 == op2) {
            return TRUE;
        } else {
            return FALSE;
        }
    } 
    else {
        return ERROR;
    }
}
