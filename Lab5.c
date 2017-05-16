#include "digraphorthogonallist.h"
int dol_equal_p(void* d1, void* d2)
{
    return *(char*)d1 == *(char*)d2;
}
char ch[2];
char* dol_print(void* d)
{

    ch[0] = *(char*)d;
    ch[1] = '\0';
    return ch;
}
int Lab5()
{
    DigraphOrthogonalList* dol;
    if(!DOL_Init(&dol,20,dol_equal_p,dol_print))
    {
        printf("%s",DOL_getLastError());
        return 0;
    }
    char c,d;
    char s;
    char filename[FILENAME_MAX];
    FILE* fd;
    do
    {
        printf("�Ƿ���ļ����룿Y/N\n");
        scanf("%c",&s);
        fflush(stdin);
        if(s=='Y'||s=='y')
        {
            printf("�������ļ�����\n");
            scanf("%s",filename);
            fflush(stdin);
            if((fd = fopen(filename,"r")))
            {
                while(!feof(fd))
                {
                    fscanf(fd,"%c - %c\n",&c,&d);
                    if(c!='#')
                    {
                        if(!DOL_AddArc(dol,&c,&d,1,1))
                        {
                            printf("%s\n",DOL_getLastError());
                        }
                    }
                    else if(d!='#')
                    {
                        if(DOL_AddVertice(dol,&d,1)==-1)
                        {
                            printf("%s\n",DOL_getLastError());
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }else
            {
                printf("�ļ��򿪴���\n");
            }
        }
        else
        {
            do
            {
                printf("�����뻡���磺a - b���������붥�㣨��# - a����"
                       "����# - #������\n");
                scanf("%c - %c",&c,&d);
                fflush(stdin);
                if(c!='#')
                {
                    if(!DOL_AddArc(dol,&c,&d,1,1))
                    {
                        printf("%s\n",DOL_getLastError());
                    }
                }
                else if(d!='#')
                {
                    if(DOL_AddVertice(dol,&d,1)==-1)
                    {
                        printf("%s\n",DOL_getLastError());
                    }
                }
            }while(!(c=='#'&&d=='#'));
        }
        int rst = DOL_DGA_p(dol);
        if(rst == -1)
        {
            printf("%s\n",DOL_getLastError());
        }
        else if(rst == 1)
        {
            printf("�жϽ�����������޻�ͼ\n");
        }
        else
        {
            printf("�жϽ�������������޻�ͼ\n");
        }
        if(!DOL_Clear(dol))
        {
            printf("%s\n",DOL_getLastError());
            return 0;
        }
        printf("�Ƿ������Y/N\n");
        fflush(stdin);
        scanf("%c",&s);
        fflush(stdin);
    }while(s!='N'&&s!='n');
    printf("���Խ���\n");
    return 0;
}
