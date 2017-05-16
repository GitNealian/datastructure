#include "Queue.h"
const char* MSG_Que_Error_PARAINVALID = "无效参数";
const char* MSG_Que_Error_NULL = "不存在错误";
const char* MSG_Que_Error_FULLQUEUE = "队列已满";
const char* MSG_Que_Error_EMPTYQUEUE = "队列为空";
const char* MSG_Que_Error_MALLOCFAILUE = "动态分配内存时出错";

enum Que_Error{Que_Error_NULL,Que_Error_PARAINVALID,Que_Error_FULLQUEUE,
                Que_Error_EMPTYQUEUE,Que_Error_MALLOCFAILUE} QueLastError = Que_Error_NULL;
//函数名：QueueInit
//功  能：初始化队列
//输  入：指向队列指针的指针q，队列最大长度max_len
//输  出：成功初始化返回1，否则返回0
int QueueInit(Queue** q,int max_len,int type)
{
    if(max_len<1||type<1||q==NULL)
    {
        QueLastError = Que_Error_PARAINVALID;
        return 0;
    }
    *q = (Queue*)malloc(sizeof(Queue));
    if(q==NULL)
    {
        QueLastError = Que_Error_MALLOCFAILUE;
        return 0;
    }
    //初始化队列
    (*q)->entrance = NULL;
    (*q)->exit = NULL;
    (*q)->length = 0;
    (*q)->MAX_LEN = max_len;
    (*q)->type = type;
    return 1;
}
//函数名：EnQueue
//功  能：将数据入队
//输  入：要加入的队列q，要插入的数据的指针
//输  出：成功返回1，否则返回0
int EnQueue(Queue* q, void *d)
{
    if(q == NULL)
    {
        QueLastError = Que_Error_PARAINVALID;
        return 0;
    }
    if(q->length>=q->MAX_LEN)//队列长度限制
    {
        QueLastError = Que_Error_FULLQUEUE;
        return 0;
    }
    Que_node* node = (Que_node*)malloc(sizeof(Que_node));//产生新结点
    if(node == NULL)
    {
        QueLastError = Que_Error_MALLOCFAILUE;
        return 0;
    }

    node->next = NULL;
    node->data = malloc(sizeof(q->type));//为新结点的数据域分配内存
    if(node->data==NULL)
    {
        free(node);
        QueLastError = Que_Error_MALLOCFAILUE;
        return 0;
    }
    memcpy(node->data,d,q->type);//复制数据
    if(q->entrance==NULL)//如果队列为空，队列出口和入口均设为此新结点
    {
        q->entrance = node;
        q->exit = node;
    }
    else //否则新结点为新的入口
    {
        q->entrance->next = node;
        q->entrance = node;
    }
    q->length++;//队列长度加1
    return 1;
}
//函数名：DeQueue
//功  能：将元素移出队列
//输  入：目标队列q，接收元素的指针d
//输  出：成功返回1，否则返回0
int DeQueue(Queue* q, void* d)
{
    if(q == NULL)
    {
        QueLastError = Que_Error_PARAINVALID;
        return 0;
    }

    if(q->length<=0)//没有元素在队列中
    {
        QueLastError = Que_Error_EMPTYQUEUE;
        return 0;
    }
    Que_node* p = q->exit;//前面排除了空队列，所以q->exit一定不为NULL
    if(d!=NULL)//d可以为NULL，此时表明只出队，不接收数据
        memcpy(d,q->exit->data,q->type);//复制数据
    free(q->exit->data);//清理内存
    q->exit = q->exit->next;//指向新的出口
    free(p);//清理内存
    q->length--;//长度减1
    if(q->length==0)//如果队列空了，将入口置为空，出口此时已经为空
    {
        q->entrance = NULL;
    }
    return 1;
}
//函数名：DestoryQueue
//功  能：销毁一个队列
//输  入：要销毁的队列q
//输  出：成功返回1，否则返回0
int DestoryQueue(Queue* q)
{
    if(q==NULL)
    {
        QueLastError = Que_Error_PARAINVALID;
        return 0;
    }

    while(DeQueue(q,NULL));//清空队列
    free(q);//删除队列
    return 1;
}
const char* QueGetLastError()
{
    switch (QueLastError)
    {
    case Que_Error_NULL:
        return MSG_Que_Error_NULL;
        break;
    case Que_Error_PARAINVALID:
        return MSG_Que_Error_PARAINVALID;
        break;
    case Que_Error_FULLQUEUE:
        return MSG_Que_Error_FULLQUEUE;
        break;
    case Que_Error_EMPTYQUEUE:
        return MSG_Que_Error_EMPTYQUEUE;
        break;
    case Que_Error_MALLOCFAILUE:
        return MSG_Que_Error_MALLOCFAILUE;
        break;
    default:
        return MSG_Que_Error_NULL;
        break;
    }
}
