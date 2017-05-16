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
        printf("是否从文件输入？Y/N\n");
        scanf("%c",&s);
        fflush(stdin);
        if(s=='Y'||s=='y')
        {
            printf("请输入文件名：\n");
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
                printf("文件打开错误\n");
            }
        }
        else
        {
            do
            {
                printf("请输入弧（如：a - b），或输入顶点（如# - a），"
                       "输入# - #结束：\n");
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
            printf("判断结果：是有向无环图\n");
        }
        else
        {
            printf("判断结果：不是有向无环图\n");
        }
        if(!DOL_Clear(dol))
        {
            printf("%s\n",DOL_getLastError());
            return 0;
        }
        printf("是否继续？Y/N\n");
        fflush(stdin);
        scanf("%c",&s);
        fflush(stdin);
    }while(s!='N'&&s!='n');
    printf("测试结束\n");
    return 0;
}
