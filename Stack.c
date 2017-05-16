#include "Stack.h"
//栈初始化
int StackInit(Stack** s,unsigned char type,int size)
{
    *s = (Stack*)malloc(sizeof(Stack));
    if(*s == NULL)
        return 0;
    (*s)->type = type;
    (*s)->data = malloc(type*size);
    (*s)->size = size;
    (*s)->pos = -1;
    return 1;
}
void StackClear(Stack* s)
{
    s->pos = -1;
}
void DestroyStack(Stack* s)
{
    free(s->data);
    free(s);
}
//获得栈顶元素但不出栈
void* touch(Stack* s)
{
    void* rst;
    if(s->pos == -1)
        rst = NULL;
    else
        rst = s->data + s->pos * s->type;
    return rst;
}
void* pop(Stack* s)
{
    void* rst;
    if(s->pos == -1)
        rst = NULL;
    else
    {
        rst = s->data + s->pos * s->type;
        s->pos--;
    }
    return rst;
}
int push(Stack* s, void* d)
{
    if(s->pos == s->size-1)
    {
        return -1;
    }
    else
    {
        s->pos++;
        memcpy((char*)s->data+s->pos*s->type,d,s->type);//拷贝数据
    }
    return s->pos;
}
int EmptyStack_p(Stack* s)
{
    return s->pos == -1;
}
int FullStack_p(Stack* s)
{
    return s->pos == s->size-1;
}
