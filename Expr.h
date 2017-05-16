#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED

#include "stdio.h"
#include "Stack.h"
float ExprCalc(FILE* file, int max_len);
float ExprExecute(char* expr);
int ExprLegalP(char* expr);
char* ExprMidToPost(char* expr);
#endif // EXPR_H_INCLUDED
