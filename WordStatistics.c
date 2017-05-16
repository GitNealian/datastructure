#include "wordstatistics.h"

FILE* filep;//���ڽ�����ͳ�Ʊ�д���ļ���
WST* merge_dest;//
//���������ʺ���
void visit(void* word)
{
    if(filep!=NULL)
    {
        fprintf(filep,"%s : %d\n",((WST_WORD*)word)->word,((WST_WORD*)word)->times);
    }

}
//��һ�����ʺ���
void merge(void* word)
{
    if(merge_dest!=NULL)
    {
        WST_AddWord(merge_dest,((WST_WORD*)word)->word,
                    ((WST_WORD*)word)->times);
    }
}

//���ڶ�����������������
int sbt_sortFun(SBT_NODE* n1,SBT_NODE* n2)
{
    //�ڵ����ַ����ıȽϽ����Ϊ�ڵ�ıȽϽ��
    return strcmp(((WST_WORD*)n1->data)->word,
                  ((WST_WORD*)n2->data)->word);
}
//��������WST_Init
//���ܣ���ʼ������ͳ�Ʊ�
//���룺ָ��Ŀ�굥��ͳ�Ʊ��ָ���ָ��wst����󵥴���max_word
//������ɹ�����1�����򷵻�0
int WST_Init(WST** wst,int max_word)
{
    if(wst == NULL || max_word < 1)//��������
    {
        return 0;
    }
    *wst = (WST*)malloc(sizeof(WST));
    if(*wst == NULL)//�ڴ�������
    {
        return 0;
    }
    int i;
    //����һ���������洢����
    SBT* sbt;
    //��������ʼ��ʧ��
    if(!SBT_Init(&sbt,sbt_sortFun,max_word))
    {
        free(*wst);
        return 0;
    }
    (*wst)->sbt = sbt;

    FirstTen* ft = (FirstTen*)malloc(sizeof(FirstTen));
    if(ft == NULL)//�ڴ�������
    {
        free(*wst);
        return 0;
    }
    ft->word = (WST_WORD*)malloc(sizeof(WST_WORD));
    if(ft->word == NULL)//�ڴ�������
    {
        free(ft);
        free(*wst);
        return 0;
    }
    ft->word->word = NULL;
    ft->word->times = 0;
    ft->pre = NULL;
    FirstTen* tmp = ft;
    (*wst)->first = ft;
    //����һ��˫����������ά�����ִ���ǰʮ�ĵ���
    //Ϊ�˱��ڲ���first��㲻���ڴ洢
    for(i = 0; i < 10; i++)
    {
       ft  = (FirstTen*)malloc(sizeof(FirstTen));
       if(ft == NULL)//�ڴ�������
       {
           ft = (*wst)->first;
           while(ft!=NULL)
           {
               tmp = ft->next;
               free(ft->word);
               free(ft);
               ft = tmp;
           }
           return 0;
       }
       ft->word = (WST_WORD*)malloc(sizeof(WST_WORD));
       if(ft->word == NULL)//�ڴ�������
       {
           free(ft);
           ft = (*wst)->first;
           while(ft!=NULL)
           {
               tmp = ft->next;
               free(ft->word);
               free(ft);
               ft = tmp;
           }
           return 0;
       }
       ft->word->word = NULL;
       ft->word->times = 0;
       ft->pre = tmp;
       ft->next = NULL;
       tmp->next = ft;
       tmp = ft;
    }
    (*wst)->last = ft;
    return 1;
}
//��������WST_InsertWord
//���ܣ�������ͳ�Ʊ������һ���µ���
//���룺Ŀ�굥��ͳ�Ʊ�wst������ӵĵ���word
//������ɹ�����1�����򷵻�0
int WST_InsertWord(WST* wst,WST_WORD* word)
{
    //�˺���ֻ�ڱ��ļ��е��ã��Ͳ������������
    //�ڴ˵����У�word�Ƕ��еı�����������ʱ����
    //���Կ�ֱ��������ָ��
    if(!SBT_AddNode(wst->sbt,word))//��������Ӵ���
    {
        return 0;
    }
    return 1;
}
//��������WST_FirstTenUpdate
//���ܣ����µ���ͳ�Ʊ��ǰʮ��
//���룺Ŀ�굥��ͳ�Ʊ�wst�������µĵ���word
//������ɹ�����1�����򷵻�0
int WST_FirstTenUpdate(WST* wst,WST_WORD* word)
{
    if(wst == NULL || word == NULL)
    {
        return 0;
    }
    FirstTen* front = wst->first->next;

    while(front!=NULL)
    {
        //���word�Ѿ���ǰʮ����ֻ�迼���Ƿ���Ҫǰ��
        if(front->word->word!=NULL &&
                strcmp(front->word->word,word->word)==0)
        {
            FirstTen* pre = front->pre;
            //��ǰ�Ƚϡ�����
            while(pre!=wst->first
                  && (pre->word->times < front->word->times
                  || (pre->word->times == front->word->times
                  && strcmp(pre->word->word,front->word->word)>0)))
            {
                pre = pre->pre;
            }
            //��ʱpreΪ��һ����С��front�Ľ��
            //���pre����frontǰһ���Ļ��Ͳ���Ҫ�ƶ���
            if(pre!=front->pre)
            {
                //���front�����һ��
                //�����ǲ�����next����
                if(front == wst->last)
                {
                    front->pre->next = front->next;
                    wst->last = front->pre;
                }
                else
                {
                    front->pre->next = front->next;
                    //front�������һ��
                    //������next����ǰһ�����ָ��front��
                    //ǰһ�����
                    front->next->pre = front->pre;
                }
                //���潫front��ѭ��������ɾ��
                //���潫front���뵽pre����
                front->next = pre->next;
                pre->next->pre = front;
                front->pre = pre;
                pre->next = front;
            }
            return 1;
        }
        front = front->next;
    }
    //���洦��word����ǰʮ���е����
    front = wst->first->next;
    while(front!=NULL)
    {
        //���word�н���ǰʮ���ʸ�
        if(word->times>front->word->times ||(
                    front->word->times == word->times
                       && strcmp(word->word,front->word->word)<0))
        {
            //��ʱfront�ǵ�һ��С��word�Ľ��
            FirstTen* new_ft = (FirstTen*)malloc(sizeof(FirstTen));
            if(new_ft == NULL)//�ڴ�������
            {
                return 0;
            }
            new_ft->word = word;
            //���½����뵽front֮ǰ
            new_ft->pre = front->pre;
            front->pre->next = new_ft;
            new_ft->next = front;
            front->pre = new_ft;
            //ԭ�������һ���˳�ǰʮ����
            FirstTen* garbage = wst->last;
            //ԭ���ĵ�����Ϊ���
            wst->last = wst->last->pre;
            wst->last->next = NULL;
            free(garbage);//����ԭ�����һ����ռ�ڴ�
            break;
        }
        front = front->next;
    }
    return 1;
}
//��������WST_AddWord
//���ܣ�������ͳ�Ʊ������һ��������
//���룺Ŀ�굥��ͳ�Ʊ�Ҫ��ӵĵ����ַ���
//������ɹ�����1�����򷵻�0
int WST_AddWord(WST* wst, char* word, int times)
{
    if(wst == NULL || word == NULL)//��������
    {
        return 0;
    }
    SBT_NODE* node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(node == NULL)//�ڴ�������
    {
        return 0;
    }
    int size = strlen(word)+1;

    WST_WORD* new_word = (WST_WORD*)malloc(sizeof(WST_WORD));
    if(new_word == NULL)//�ڴ�������
    {
        free(node);
        return 0;
    }
    new_word->word = (char*)malloc(size);
    if(new_word->word == NULL)//�ڴ�������
    {
        free(node);
        free(new_word);
        return 0;
    }
    memcpy(new_word->word,word,size);//�������ݵ����ڴ�
    new_word->times = times;//�µ��ʳ��ִ���
    node->data = new_word;
    //���洴���˶��������
    //����֮ǰд��SBT_Search��Ҫ����SBT_NODE*��
    //����Ϊ��ʱ�ȽϺ��������ĽӿڵĲ���������SBT_NODE*��
    //���Դ˴���Ҫ����������������ڲ���
    //ʵ���е����Ѿ������ڶ������е�����Ӷ�
    //��Ҫ�����������Ϊ���������������������ʱ���ϵĿ���
    SBT_NODE* index = SBT_Search(wst->sbt,node);
    if(index!=NULL)//���word�Ѵ��ڣ�word�Ĵ�����1
    {
        ((WST_WORD*)(index->data))->times += times;
        free(node);//����Ѿ�������ônode�Ͳ�����Ҫ��
        free(new_word);
    }
    else//�������˵���
    {
        index = node;
        WST_InsertWord(wst,new_word);
    }
    WST_FirstTenUpdate(wst,(WST_WORD*)index->data);//����ǰʮ��word
    return 1;
}
//��������WST_ToFile
//���ܣ�������ͳ�ƽ�����浽�ļ���
//���룺Ŀ�굥��ͳ�Ʊ�wst��Ҫ���浽���ļ�ָ��file
//fileҪ�����дȨ��
//������ɹ�����1�����򷵻�0
int WST_ToFile(WST* wst, FILE* file)
{
    if(wst==NULL || file ==NULL)
    {
        return 0;
    }
    filep = file;
    SBT_Taversal_Inorder(wst->sbt,visit);
    filep = NULL;
    return 0;
}
//��������WST_LoadFromFile
//���ܣ����ļ��ж�ȡ�����ɵ���ͳ�Ʊ�
//���룺Ŀ���ļ�fd��Ŀ�굥��ͳ�Ʊ�wst
//������ɹ�����1�����򷵻�0
int WST_LoadFromFile(FILE* fd,WST* wst)
{
    char word[30];
    int times;
    while(!feof(fd))
    {
        fscanf(fd,"%s : %d",word,&times);
        WST_AddWord(wst,word,times);
    }
    return 1;
}
//��������WST_Search
//���ܣ��ڵ���ͳ�Ʊ�wst�в��ҵ���word
//���룺Ŀ�굥��ͳ�Ʊ�wst��Ҫ���ҵĵ����ַ���word
//������ҵ�������WST_WORDָ�룬���򷵻�NULL
WST_WORD* WST_Search(WST* wst, char* word)
{

    if(wst == NULL || word == NULL)//��������
    {
        return 0;
    }
    SBT_NODE* node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(node == NULL)//�ڴ�������
    {
        return 0;
    }

    WST_WORD* new_word = (WST_WORD*)malloc(sizeof(WST_WORD));
    if(new_word == NULL)//�ڴ�������
    {
        free(node);
        return 0;
    }
    new_word->word = word;
    new_word->times = 1;
    node->data = new_word;
    SBT_NODE* index = SBT_Search(wst->sbt,node);
    if(index!=NULL)
    {
        free(node);
        free(new_word);
        return (WST_WORD*)index->data;
    }
    return NULL;
}
//��������WST_Merge
//���ܣ�����������ͳ�Ʊ����ϵ�һ��
//���룺Ŀ�굥��ͳ�Ʊ�dest��Դ����ͳ�Ʊ�src
//�����1

int WST_Merge(WST* dest,WST* src)
{
    merge_dest = dest;
    SBT_Taversal_Inorder(src->sbt,merge);
    merge_dest = NULL;
    return 1;
}

//��������WST_Destory
//���ܣ�����Ŀ�굥��ͳ�Ʊ�
//���룺Ŀ�굥��ͳ�Ʊ�
//������ɹ�����1�����򷵻�0
int WST_Destory(WST* wst)
{
    if(wst==NULL)
    {
        return 0;
    }
    SBT_Destory(&(wst->sbt));
    FirstTen* tmp;
    FirstTen* garbage;
    tmp = wst->first->next;
    garbage = wst->first;
    while(garbage!=NULL)
    {
        tmp = tmp->next;
        free(garbage);
        garbage = tmp;

    }
    free(wst);
    return 1;
}
