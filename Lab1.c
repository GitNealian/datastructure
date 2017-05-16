#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "windows.h"
typedef struct node
{
    int index;
    int data;
    struct node* next;
}node;
typedef struct
{
    int length;
    struct node* head;
}List;
List* MakeList(char*);
void DestoryList(List* );
node* Adjmax(List*, int);
void make_test_file();
node* Adjmax2(List*,int );

int Lab1()
{
    List* L = NULL;
    char file[FILENAME_MAX];
    char* filename = NULL;
    unsigned long int t;
    char c;
    int k;
    do
    {
        printf("是否从文件读取数据？Y/N\n");
        fflush(stdin);
        c = getchar();
        if(c == 'Y' || c == 'y')
        {
            printf("请输入文件路径：\n");
            scanf("%s",file);
            filename = file;
        }
        else
        {
            filename = NULL;
        }
        L = MakeList(filename);
        if(L!=NULL)
        {
            printf("请输入k：\n");
            scanf("%d",&k);

            if(k>0)
            {
                t = GetTickCount();
                node* rst = Adjmax(L,k);
                printf("算法1耗时：%ld ms\n",GetTickCount()-t);
                if(rst!=NULL)
                {
                    printf("(%d, %d)\n",rst->index,rst->data);
                }
                t = GetTickCount();
                rst = Adjmax2(L,k);
                printf("算法2耗时：%ld ms\n",GetTickCount()-t);
                if(rst!=NULL)
                {
                    printf("(%d, %d)\n",rst->index,rst->data);
                }
                DestoryList(L);
            }
            else
            {
                printf("k不能小于1\n");
            }
        }
        else
        {
            printf("链表未成功生成");
        }
        printf("是否继续？Y/N\n");
        fflush(stdin);
        c = getchar();
    }while(c=='Y' || c == 'y');
    return 0;
}

void DestoryList(List* L)
{
    node* tmp;
    L->length = 0;
    while(L->head!=NULL)
    {
        tmp = L->head->next;
        free(L->head);
        L->head = tmp;
    }
}
List* MakeList(char* filename)
{
    List* L = (List*)malloc(sizeof(List));
    L->head = NULL;
    L->length = 0;
    node* n = NULL;
    int index = 0;
    int data = 0;

    if(filename == NULL)
    {
        printf("请输入结点数据,输入-1结束:\n");
        scanf("%d",&data);
        if(data>=0 && data <100)
        {
            n = (node*)malloc(sizeof(node));
            n->data = data;
            n->next = NULL;
            n->index = index;
            index++;
            L->head = n;
            L->length++;
        }
        while(data!=-1)
        {
            scanf("%d",&data);
            if(data>=0 && data <100)
            {
                n->next = (node*)malloc(sizeof(node));
                n = n->next;
                n->index = index;
                index++;
                n->data = data;
                n->next = NULL;
                L->length++;
            }
        }
    }
    else
    {
        FILE* fd = fopen(filename,"r");
        if(fd)
        {
            fscanf(fd,"%d",&data);
            if(data>=0 && data <100)
            {
                n = (node*)malloc(sizeof(node));
                n->data = data;
                n->next = NULL;
                n->index = index;
                index++;
                L->head = n;
                L->length++;
            }
            while(data!=-1&&!feof(fd))
            {
                fscanf(fd,"%d",&data);
                if(data>=0 && data <100)
                {
                    n->next = (node*)malloc(sizeof(node));
                    n = n->next;
                    n->index = index;
                    index++;
                    n->data = data;
                    n->next = NULL;
                    L->length++;
                }
            }
            fclose(fd);
        }
        else
        {
            printf("打开文件时发生错误\n");
        }
    }
    return L;
}


node* Adjmax(List* L,int k)
{
    node* q = L->head;
    node* p = q;
    node* rst = q;
    int i = 0;
    int sum = 0;
    int sum_tmp = 0;
    if(k > L->length)
    {
        printf("k不能超过链表的长度，length = %d",L->length);
        return NULL;
    }
    while(i<k)
    {
        sum_tmp = sum_tmp + q->data;
        q = q->next;
        i++;
    }
    sum = sum_tmp;
    while(q!=NULL)
    {
        sum_tmp = sum_tmp - p->data + q->data;
        if(sum_tmp > sum)
        {
            sum = sum_tmp;
            rst = p->next;
        }
        p = p->next;
        q = q->next;
    }
    return rst;
}

node* Adjmax2(List* L,int k)
{
    node* q = L->head;
    node* p = q;
    node* r = q;
    node* rst = q;
    int i = 0;
    int sum = 0;
    int sum_tmp = 0;
    if(k > L->length)
    {
        printf("k不能超过链表的长度，length = %d",L->length);
        return NULL;
    }
    while(i<k-1)
    {
        q = q->next;
        i++;
    }
    while(q!=NULL)
    {
        sum_tmp = 0;
        r = p;
        i = 0;
        while(i<k)
        {
            sum_tmp += p->data;
            p = p->next;
            i++;
        }
        if(sum_tmp > sum)
        {
            sum = sum_tmp;
            rst = r;
        }
        p = r->next;
        q = q->next;
    }
    return rst;
}
void make_test_file()
{
    int i = 0;
    srand(time(NULL));
    FILE* fd = fopen("test.txt","w");
    if(fd)
    {
        while(i< (int)pow(10,7))
        {
            fprintf(fd,"%d ",rand()%100);
            i++;
        }
        fprintf(fd,"%d",-1);
        fclose(fd);
    }

}
//int Adjmax(L,k)
//{
//    int i = 0;
//    int index = 0;
//    int sum = 0;
//    int sum_tmp = 0;
//    if(k>length(L))
//    {
//        return 0;
//    }
//    else
//    {
//        while(i<k-1)
//        {
//            sum += GetElem(L,i);
//            i++;
//        }
//        while(i<length(L))
//        {
//           sum_tmp = sum_temp + GetElem(L,i-k) + GetElem(L,i);
//           if(sum_tmp > sum) // 等于时取第一个
//           {
//               sum = sum_tmp;
//               index = i-k+1;
//           }
//           i++:
//        }
//    }
//    return index;
//}
