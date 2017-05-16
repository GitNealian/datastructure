#include "Queue.h"
#define MAX_LEN 10
int Lab3()
{
    Queue* q;
    char c;
    if(!QueueInit(&q,MAX_LEN,QUE_TYPE_CHAR))
        return -1;
    do
    {
        printf("������һ���ַ���'0'���ӣ�'@'ȫ�����ӣ�'#'��������������ӣ���\n");
        scanf("%c",&c);fflush(stdin);
        switch(c)
        {
        case '0':if(DeQueue(q,&c)) printf("���ӣ�%c\n",c);else printf("%s\n",QueGetLastError());break;
        case '@':
            if(!DeQueue(q,&c)) printf("%s\n",QueGetLastError());
            else
            {
                printf("���ӣ�");
                do
                {
                    printf("%c ",c);
                }while(DeQueue(q,&c));
                printf("\n");
            }
        case '#':break;
        default:if(!EnQueue(q,&c)) printf("%s\n",QueGetLastError());
        }
    }while(c!='#');
    DestoryQueue(q);
    return 0;
}
