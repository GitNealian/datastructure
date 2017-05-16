#include "Queue.h"
const char* MSG_Que_Error_PARAINVALID = "��Ч����";
const char* MSG_Que_Error_NULL = "�����ڴ���";
const char* MSG_Que_Error_FULLQUEUE = "��������";
const char* MSG_Que_Error_EMPTYQUEUE = "����Ϊ��";
const char* MSG_Que_Error_MALLOCFAILUE = "��̬�����ڴ�ʱ����";

enum Que_Error{Que_Error_NULL,Que_Error_PARAINVALID,Que_Error_FULLQUEUE,
                Que_Error_EMPTYQUEUE,Que_Error_MALLOCFAILUE} QueLastError = Que_Error_NULL;
//��������QueueInit
//��  �ܣ���ʼ������
//��  �룺ָ�����ָ���ָ��q��������󳤶�max_len
//��  �����ɹ���ʼ������1�����򷵻�0
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
    //��ʼ������
    (*q)->entrance = NULL;
    (*q)->exit = NULL;
    (*q)->length = 0;
    (*q)->MAX_LEN = max_len;
    (*q)->type = type;
    return 1;
}
//��������EnQueue
//��  �ܣ����������
//��  �룺Ҫ����Ķ���q��Ҫ��������ݵ�ָ��
//��  �����ɹ�����1�����򷵻�0
int EnQueue(Queue* q, void *d)
{
    if(q == NULL)
    {
        QueLastError = Que_Error_PARAINVALID;
        return 0;
    }
    if(q->length>=q->MAX_LEN)//���г�������
    {
        QueLastError = Que_Error_FULLQUEUE;
        return 0;
    }
    Que_node* node = (Que_node*)malloc(sizeof(Que_node));//�����½��
    if(node == NULL)
    {
        QueLastError = Que_Error_MALLOCFAILUE;
        return 0;
    }

    node->next = NULL;
    node->data = malloc(sizeof(q->type));//Ϊ�½�������������ڴ�
    if(node->data==NULL)
    {
        free(node);
        QueLastError = Que_Error_MALLOCFAILUE;
        return 0;
    }
    memcpy(node->data,d,q->type);//��������
    if(q->entrance==NULL)//�������Ϊ�գ����г��ں���ھ���Ϊ���½��
    {
        q->entrance = node;
        q->exit = node;
    }
    else //�����½��Ϊ�µ����
    {
        q->entrance->next = node;
        q->entrance = node;
    }
    q->length++;//���г��ȼ�1
    return 1;
}
//��������DeQueue
//��  �ܣ���Ԫ���Ƴ�����
//��  �룺Ŀ�����q������Ԫ�ص�ָ��d
//��  �����ɹ�����1�����򷵻�0
int DeQueue(Queue* q, void* d)
{
    if(q == NULL)
    {
        QueLastError = Que_Error_PARAINVALID;
        return 0;
    }

    if(q->length<=0)//û��Ԫ���ڶ�����
    {
        QueLastError = Que_Error_EMPTYQUEUE;
        return 0;
    }
    Que_node* p = q->exit;//ǰ���ų��˿ն��У�����q->exitһ����ΪNULL
    if(d!=NULL)//d����ΪNULL����ʱ����ֻ���ӣ�����������
        memcpy(d,q->exit->data,q->type);//��������
    free(q->exit->data);//�����ڴ�
    q->exit = q->exit->next;//ָ���µĳ���
    free(p);//�����ڴ�
    q->length--;//���ȼ�1
    if(q->length==0)//������п��ˣ��������Ϊ�գ����ڴ�ʱ�Ѿ�Ϊ��
    {
        q->entrance = NULL;
    }
    return 1;
}
//��������DestoryQueue
//��  �ܣ�����һ������
//��  �룺Ҫ���ٵĶ���q
//��  �����ɹ�����1�����򷵻�0
int DestoryQueue(Queue* q)
{
    if(q==NULL)
    {
        QueLastError = Que_Error_PARAINVALID;
        return 0;
    }

    while(DeQueue(q,NULL));//��ն���
    free(q);//ɾ������
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
