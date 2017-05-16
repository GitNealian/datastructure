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
    printf("��ѡ��һ�������\n1����Ӽ�¼ "
           "2���򿪼�¼ 3���˳�\n");
    option = getchar();
    fflush(stdin);
    switch(option)
    {
    case '1': state = S_ADD;break;
    case '2': state = S_OPEN;break;
    case '3': state = S_EXIT;break;
    default: printf("������Ч��");
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
    printf("�������ļ�����");
    gets(filename);
    fflush(stdin);

    system("cls");
    printf("������Ӽ�¼��%s\n",filename);
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
        fprintf(flog,"�ɹ����һ����¼��%s\n\n",
                filename);

        //���������Ϣ
        printf("�ɹ����һ����¼��%s,��ʱ%dms\n",
               filename,tc);
        printf("ͳ�ƽ�����£�");
        printf("����¼��ͳ�Ʋ��ظ�����%d����"
               "���г��ִ���ǰʮ���ĵ��ʷֱ�Ϊ��\n",
               wst->sbt->len);
        ft = wst->first->next;
        while(ft!=NULL)
        {
            printf("%s: %d��\n",ft->word->word,
                   ft->word->times);
            ft = ft->next;
        }
        //д���ļ�
        char directory[MAX_PATH] = "record\\";
        char* tmp = strcat(directory,filename);
        while (*tmp!='.') {
            tmp++;
        }
        *tmp = '\0';
        FILE* fd = fopen(directory,"w");
        WST_ToFile(wst,fd);
        fclose(fd);
        //״̬ת��
        printf("�Ƿ�򿪴˼�¼�Խ��в�ѯ��Y/N ");
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
        fprintf(flog,"��Ӽ�¼%sʧ��\n\n",filename);

        printf("���ʧ�ܡ�\n");
        Sleep(500);
        state = S_SURFACE;
    }
    return state;
}
int lab6_open()
{
    char recordname[FILENAME_MAX];
    printf("�������¼����\n");
    gets(recordname);
    //��ѯ����¼
    if(strcmp(recordname,"wst_main") == 0)
    {
        wst_current = wst_main;
        return S_SEARCH;
    }

    char directory[FILENAME_MAX] = "record\\";
    char* tmp = strcat(directory,recordname);
    //�򿪼�¼
    FILE* fd = fopen(tmp,"r");
    if(fd == NULL)
    {
        writeLogTime();
        fprintf(flog,"�򿪼�¼ %s ʧ��\n\n",recordname);
        printf("�򿪼�¼ %s ʧ�ܡ�\n",recordname);
        Sleep(300);
        return S_SURFACE;
    }
    printf("���ڴ򿪼�¼�����Ե�\n");
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

    printf("������Ҫ��ѯ�ĵ��ʣ�");
    gets(search_word);
    if(strcmp(search_word,"wst_back") == 0)
    {
        return S_BACK;
    }
    if(strcmp(search_word,"wst_top_ten") == 0)
    {
        printf("����¼��ͳ�Ʋ��ظ�����%d����"
               "���г��ִ���ǰʮ���ĵ��ʷֱ�Ϊ��\n",
               wst_current->sbt->len);
        ft = wst_current->first->next;
        while(ft!=NULL)
        {
            printf("%s: %d��\n",ft->word->word,
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
        printf("�õ��ʼ�¼���ҵ�����ʱ%dms\n",tc);
        printf("%s : %d��\n",search_word,
               search_rst->times);
    }
    else
    {
        printf("����¼��δ�ҵ��õ��ʡ�\n");
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
    fprintf(flog,"����ͳ�Ʊ�����\n\n");
    //��ʼ����������ͳ�Ʊ�
    WST_Init(&wst_main,200000);//������ͳ�Ʊ��������м�¼����Ϣ
    WST_Init(&wst_current,200000);//��������ǰ�򿪼�¼����Ϣ
    printf("���ڼ�������¼�����Ե�\n");
    FILE* fmain = fopen(MAIN_RECORD,"r");
    if(fmain!=NULL)
    {
        WST_LoadFromFile(fmain,wst_main);
        fclose(fmain);
    }//���ļ��ж�ȡ����¼


    int state = 0;//��״̬
    //��ʼ��ѭ��
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
        case S_BACK://�رյ�ǰ��¼��ɾ����Ӧ�ĵ���ͳ�Ʊ�
            if(wst_current!=wst_main)
                WST_Destory(wst_current);
            wst_current = NULL;
            state = S_SURFACE;
            break;
        case S_EXIT:
            printf("����������\n");
            writeLogTime();
            fprintf(flog,"��������\n\n");
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
//���һ����¼
WST* Lab6_AddRecord(char* file)
{
    FILE* fd;
    char directory[PATH_MAX] = "record\\";
    char* dic = strcat(directory,file);
    fd = fopen(dic,"r");
    if(fd)
    {
        writeLogTime();
        fprintf(flog,"��Ӽ�¼%sʱʧ�ܣ���¼�Ѵ���\n\n",dic);
        fclose(fd);
        return NULL;
    }

    WST* wst;
    if(!WST_Init(&wst,200000))
    {
        writeLogTime();
        fprintf(flog,"��Ӽ�¼%sʱʧ�ܣ�"
                     "��ʼ������ͳ�Ʊ�ʧ��\n\n",dic);
        return NULL;
    }
    file = strcat(file,".txt");
    fd = fopen(file,"r");
    if(!fd)
    {
        writeLogTime();
        fprintf(flog,"��Ӽ�¼%sʱʧ�ܣ�"
                     "�ļ���ʧ��\n\n",dic);
        return NULL;
    }

    char ch;
    char word_buff[MAX_WORD+1];
    int pos;
    int state = INITIAL;
    //����״̬����ȡ���ʲ���ӵ�����ͳ�Ʊ���
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
