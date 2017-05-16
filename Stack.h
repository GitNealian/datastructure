#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define STACK_SIZE 128
#define TYPE_FLOAT 4
#define TYPE_CHAR 1
#define TYPE_POINTER 4
#define TYPE_INT 4
typedef struct Stack
{
    unsigned char type;
    int pos;
    int size;
    void* data;
}Stack;

int EmptyStack_p(Stack* s);
int FullStack_p(Stack* s);
int push(Stack* s, void* d);
void* touch(Stack* s);
void* pop(Stack* s);
void DestroyStack(Stack* s);
void StackClear(Stack* s);
int StackInit(Stack** s,unsigned char type, int stack_size);
#endif // STACK_H_INCLUDED
