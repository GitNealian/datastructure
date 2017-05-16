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
        printf("请输入一个字符（'0'出队，'@'全部出队，'#'结束，其它：入队）：\n");
        scanf("%c",&c);fflush(stdin);
        switch(c)
        {
        case '0':if(DeQueue(q,&c)) printf("出队：%c\n",c);else printf("%s\n",QueGetLastError());break;
        case '@':
            if(!DeQueue(q,&c)) printf("%s\n",QueGetLastError());
            else
            {
                printf("出队：");
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
