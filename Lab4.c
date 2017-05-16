#include "SortedBinaryTree.h"
#define MAX_LEN 20
int SBT_sortFUN(SBT_NODE* node, SBT_NODE* new_node)
{
    return strcmp((char*)node->data,(char*)new_node->data);
}
void SBT_visit(void* d)
{
    printf("%s ",(char*)d);
}
int Lab4()
{
    SBT* sbt;
    char str[20];
    SBT_Init(&sbt,SBT_sortFUN,MAX_LEN);
    char c;
    char file[FILENAME_MAX];
    char* filename = NULL;
    FILE* fd;
    do
    {
        printf("是否从文件中读取？Y/N\n");
        fflush(stdin);
        c = getchar();
        fflush(stdin);
        if(c == 'Y'||c == 'y')
        {
            printf("请输入文件名：\n");
            gets(file);
            filename = file;
            fd = fopen(file,"r");
        }
        else
        {
            filename = NULL;
            printf("请输入一个英文句子（无标点），以空格加#号结束：\n");
        }
        do
        {
            fd = filename?fd:stdin;
            if(!fd)
            {
                printf("打开文件出错\n");
                break;
            }
            fscanf(fd,"%s",str);
            if(str[0]!='#')
            {
                if(!SBT_Insert(sbt,str,strlen(str)+1))
                {
                    printf("%s\n",SBT_getLastError());
                }
            }
        }while(str[0]!='#');
        if(!SBT_Taversal_Inorder(sbt,SBT_visit))
        {
            printf("%s\n",SBT_getLastError());
        }
        printf("\n");
        if(!SBT_Clear(sbt))
        {
            printf("%s\n",SBT_getLastError());
        }
        printf("是否继续？(Y/N)\n");
        fflush(stdin);
        c = getchar();
        fflush(stdin);
    }while(c!='N'&&c!='n');
    printf("测试结束\n");
    return 0;
}
