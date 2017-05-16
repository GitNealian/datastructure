#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define QUE_TYPE_CHAR 1

typedef struct Que_node
{
    void* data;
    struct Que_node* next;
}Que_node;
typedef struct Queue
{
    Que_node* entrance;
    Que_node* exit;
    int length;
    int MAX_LEN;
    int type;
}Queue;
//函数名：QueueInit
//功  能：初始化队列
//输  入：指向队列指针的指针q，队列最大长度max_len
//输  出：成功初始化返回1，否则返回0
int QueueInit(Queue** q,int max_len,int type);
//函数名：EnQueue
//功  能：将数据入队
//输  入：要加入的队列q，要插入的数据的指针
//输  出：成功返回1，否则返回0
int EnQueue(Queue* q, void *d);
//函数名：DeQueue
//功  能：将元素插入到队列
//输  入：要插入的目标队列q，要插入的数据指针d
//输  出：成功返回1，否则返回0
int DeQueue(Queue* q, void* d);
//函数名：DestoryQueue
//功  能：销毁一个队列
//输  入：要销毁的队列q
//输  出：成功返回1，否则返回0
int DestoryQueue(Queue* q);
const char* QueGetLastError();
#endif // QUEUE_H_INCLUDED
