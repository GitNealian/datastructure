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
        printf("�Ƿ���ļ��ж�ȡ��Y/N\n");
        fflush(stdin);
        c = getchar();
        fflush(stdin);
        if(c == 'Y'||c == 'y')
        {
            printf("�������ļ�����\n");
            gets(file);
            filename = file;
            fd = fopen(file,"r");
        }
        else
        {
            filename = NULL;
            printf("������һ��Ӣ�ľ��ӣ��ޱ�㣩���Կո��#�Ž�����\n");
        }
        do
        {
            fd = filename?fd:stdin;
            if(!fd)
            {
                printf("���ļ�����\n");
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
        printf("�Ƿ������(Y/N)\n");
        fflush(stdin);
        c = getchar();
        fflush(stdin);
    }while(c!='N'&&c!='n');
    printf("���Խ���\n");
    return 0;
}
