#include "wordstatistics.h"
#include "windows.h"
#include "time.h"


#define INITIAL 0
#define LETTER 1
#define TOOLONG 2

#define MAX_WORD 30

#define S_SURFACE 0
#define S_ADD 1
#define S_OPEN 2
#define S_SEARCH 3
#define S_BACK 4
#define S_EXIT 5

#define MAIN_RECORD "record\\wst_main"
void writeLogTime();
WST* Lab6_AddRecord(char* file);
FILE* flog;
WST* wst_main;
WST* wst_current;
int lab6_surface()
{
    char option;
    int state;
    system("cls");
    printf("请选择一项操作：\n1、添加记录 "
           "2、打开记录 3、退出\n");
    option = getchar();
    fflush(stdin);
    switch(option)
    {
    case '1': state = S_ADD;break;
    case '2': state = S_OPEN;break;
    case '3': state = S_EXIT;break;
    default: printf("操作无效。");
                Sleep(500);state = S_SURFACE;
    }
    return state;
}
int lab6_add()
{
    WST* wst;
    char option;
    int state;
    int tc;
    FirstTen* ft;
    FILE* fmain;

    char filename[FILENAME_MAX];
    printf("请输入文件名：");
    gets(filename);
    fflush(stdin);

    system("cls");
    printf("正在添加记录：%s\n",filename);
    tc = GetTickCount();
    wst = Lab6_AddRecord(filename);
    tc = GetTickCount()-tc;

    if(wst!=NULL)
    {
        WST_Merge(wst_main,wst);
        fmain = fopen(MAIN_RECORD,"w");
        WST_ToFile(wst_main,fmain);
        fclose(fmain);

        writeLogTime();
        fprintf(flog,"成功添加一条记录：%s\n\n",
                filename);

        //输出基本信息
        printf("成功添加一条记录：%s,耗时%dms\n",
               filename,tc);
        printf("统计结果如下：");
        printf("本记录共统计不重复单词%d个，"
               "其中出现次数前十名的单词分别为：\n",
               wst->sbt->len);
        ft = wst->first->next;
        while(ft!=NULL)
        {
            printf("%s: %d次\n",ft->word->word,
                   ft->word->times);
            ft = ft->next;
        }
        //写到文件
        char directory[MAX_PATH] = "record\\";
        char* tmp = strcat(directory,filename);
        while (*tmp!='.') {
            tmp++;
        }
        *tmp = '\0';
        FILE* fd = fopen(directory,"w");
        WST_ToFile(wst,fd);
        fclose(fd);
        //状态转移
        printf("是否打开此记录以进行查询？Y/N ");
        fflush(stdin);
        option = getchar();
        fflush(stdin);
        if(option == 'Y'||option=='y')
        {
            system("cls");
            wst_current = wst;
            state = S_SEARCH;
        }
        else
        {
            WST_Destory(wst);
            state = S_SURFACE;
        }
    }
    else
    {
        writeLogTime();
        fprintf(flog,"添加记录%s失败\n\n",filename);

        printf("添加失败。\n");
        Sleep(500);
        state = S_SURFACE;
    }
    return state;
}
int lab6_open()
{
    char recordname[FILENAME_MAX];
    printf("请输入记录名：\n");
    gets(recordname);
    //查询主记录
    if(strcmp(recordname,"wst_main") == 0)
    {
        wst_current = wst_main;
        return S_SEARCH;
    }

    char directory[FILENAME_MAX] = "record\\";
    char* tmp = strcat(directory,recordname);
    //打开记录
    FILE* fd = fopen(tmp,"r");
    if(fd == NULL)
    {
        writeLogTime();
        fprintf(flog,"打开记录 %s 失败\n\n",recordname);
        printf("打开记录 %s 失败。\n",recordname);
        Sleep(300);
        return S_SURFACE;
    }
    printf("正在打开记录，请稍等\n");
    WST_LoadFromFile(fd,wst_current);
    fclose(fd);
    system("cls");
    return S_SEARCH;
}
void writeLogTime()
{
    time_t now;
    time(&now);
    if(flog)
    {
        fprintf(flog,"%s",ctime(&now));
    }
}

int lab6_search()
{
    char search_word[30];
    FirstTen* ft;
    int tc;
    WST_WORD* search_rst;

    printf("请输入要查询的单词：");
    gets(search_word);
    if(strcmp(search_word,"wst_back") == 0)
    {
        return S_BACK;
    }
    if(strcmp(search_word,"wst_top_ten") == 0)
    {
        printf("本记录共统计不重复单词%d个，"
               "其中出现次数前十名的单词分别为：\n",
               wst_current->sbt->len);
        ft = wst_current->first->next;
        while(ft!=NULL)
        {
            printf("%s: %d次\n",ft->word->word,
                   ft->word->times);
            ft = ft->next;
        }
        return S_SEARCH;
    }
    tc = GetTickCount();
    search_rst = WST_Search(wst_current,
                            strupr(search_word));
    tc = GetTickCount() - tc;
    if(search_rst!=NULL)
    {
        printf("该单词记录已找到，耗时%dms\n",tc);
        printf("%s : %d次\n",search_word,
               search_rst->times);
    }
    else
    {
        printf("本记录中未找到该单词。\n");
    }
    printf("\n");
    return S_SEARCH;
}
int Lab6_main()
{
    flog = fopen("log.txt","at");
    if(!flog)
    {
        flog = fopen("log.txt","w");
    }
    writeLogTime();
    fprintf(flog,"单词统计表启动\n\n");
    //初始化两个单词统计表
    WST_Init(&wst_main,200000);//主单词统计表，包含所有记录的信息
    WST_Init(&wst_current,200000);//仅包含当前打开记录的信息
    printf("正在加载主记录，请稍等\n");
    FILE* fmain = fopen(MAIN_RECORD,"r");
    if(fmain!=NULL)
    {
        WST_LoadFromFile(fmain,wst_main);
        fclose(fmain);
    }//从文件中读取主记录


    int state = 0;//主状态
    //开始主循环
    while(1)
    {

        switch (state) {
        case S_SURFACE:
            state = lab6_surface();
            break;
        case S_ADD:
            state = lab6_add();
            break;
        case S_OPEN:
            state = lab6_open();
            break;
        case S_SEARCH:
            state = lab6_search();
            break;
        case S_BACK://关闭当前记录，删除相应的单词统计表
            if(wst_current!=wst_main)
                WST_Destory(wst_current);
            wst_current = NULL;
            state = S_SURFACE;
            break;
        case S_EXIT:
            printf("操作结束。\n");
            writeLogTime();
            fprintf(flog,"操作结束\n\n");
            return 0;
            break;
        default:
            state = 0;
            break;
        }
    }
    if(flog)
    {
        fclose(flog);
    }
}
int Letter_p(char ch)
{
    return (ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')
            ||(ch<='0'&&ch>='9')||ch=='_';
}
//添加一个记录
WST* Lab6_AddRecord(char* file)
{
    FILE* fd;
    char directory[PATH_MAX] = "record\\";
    char* dic = strcat(directory,file);
    fd = fopen(dic,"r");
    if(fd)
    {
        writeLogTime();
        fprintf(flog,"添加记录%s时失败：记录已存在\n\n",dic);
        fclose(fd);
        return NULL;
    }

    WST* wst;
    if(!WST_Init(&wst,200000))
    {
        writeLogTime();
        fprintf(flog,"添加记录%s时失败："
                     "初始化单词统计表失败\n\n",dic);
        return NULL;
    }
    file = strcat(file,".txt");
    fd = fopen(file,"r");
    if(!fd)
    {
        writeLogTime();
        fprintf(flog,"添加记录%s时失败："
                     "文件打开失败\n\n",dic);
        return NULL;
    }

    char ch;
    char word_buff[MAX_WORD+1];
    int pos;
    int state = INITIAL;
    //利用状态机抽取单词并添加到单词统计表中
    while(!feof(fd))
    {
        switch(state)
        {
        case INITIAL:
            pos = -1;
            ch = fgetc(fd);
            if(Letter_p(ch))
            {
                pos++;
                word_buff[pos] = ch & 0xdf;
                state = LETTER;
            }
            break;
        case LETTER:
            ch = fgetc(fd);
            if(Letter_p(ch))
            {
                pos++;
                word_buff[pos] = ch & 0xdf;
                if(pos>=MAX_WORD)
                {
                    state = TOOLONG;
                }
            }
            else
            {
                word_buff[pos+1] = '\0';
                WST_AddWord(wst,word_buff,1);
                state = INITIAL;
            }
            break;
        case TOOLONG:
            while (Letter_p(fgetc(fd)));
            state = INITIAL;
        }

    }
    fclose(fd);
    return wst;
}
int Lab6()
{
    Lab6_main();
    return 0;
}
