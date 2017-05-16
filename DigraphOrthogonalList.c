#include "digraphorthogonallist.h"

const char* str_dol_error_invalidpara = "������Ч";
const char* str_dol_error_mallocfaild = "�ڴ�������";
const char* str_dol_error_stack = "ջ����";
const char* str_dol_error_fulllist = "����������";
const char* str_dol_error_emptylist = "����մ���";
const char* str_dol_error_verexits = "�����Ѵ���";
const char* str_dol_error_arcexits = "���Ѵ���";
const char* str_dol_error_null = "û�д���";
enum dol_error{DOL_ERROR_INVALIDPARA,DOL_ERROR_MALLOCFAILD,
           DOL_ERROR_STACK,DOL_ERROR_FULLLIST,DOL_ERROR_EMPTYLIST,
           DOL_ERROR_VEREXITS,DOL_ERROR_ARCEXITS,DOL_ERROR_NULL}DOL_LastError;
//��������DOL_Init
//�� �ܣ���ʼ��ʮ������
//�� �룺ָ��ʮ������ָ���ָ��dol������ͼ�������size��
//       �ж�������ĺ���ָ��DolEqual_p
//�� �����ɹ�����1�����򷵻�0
int DOL_Init(DigraphOrthogonalList** dol,int size,
               int (*DolEqual_p)(void*,void*),
             char* (*DolPrint)(void*))
{
    //��������
    if(dol == NULL || size < 1 || DolEqual_p == NULL)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return 0;
    }
    *dol = (DigraphOrthogonalList*)
            malloc(sizeof(DigraphOrthogonalList));
    if(*dol == NULL)//�ڴ�������
    {
        DOL_LastError = DOL_ERROR_MALLOCFAILD;
        return 0;
    }
    //��ʼ��ʮ������
    (*dol)->size = size;
    (*dol)->pos = -1;
    (*dol)->vertices = (DOL_VERTICE*)
            malloc(sizeof(DOL_VERTICE)*size);
    if((*dol)->vertices == NULL)
    {
        free(*dol);
        DOL_LastError = DOL_ERROR_MALLOCFAILD;
        return 0;
    }
    //��������
    memset((*dol)->vertices,0,sizeof(DOL_VERTICE)*size);
    (*dol)->DolEqual_p = DolEqual_p;
    (*dol)->DolPrint = DolPrint;
    DOL_LastError = DOL_ERROR_NULL;
    return 1;
}
//��������DOL_LocateVer
//�� �ܣ���ȡ������ʮ����������е�����
//�� �룺Ŀ��ʮ������dol��Ҫ���ҵ�����ָ��data
//�� ���������ڶ�����з��������������򷵻�-1
int DOL_LocateVer(DigraphOrthogonalList* dol,
                  void* data)
{
    if(dol == NULL || data == NULL)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return 0;
    }
    int i = 0;
    while(i<=dol->pos)
    {
        //�����Ƚϣ��ʺ϶��㲻������
        //�綥����Ŀ�󣬿ɲ��������ṹ
        //��ʵ�鶥�����
        if(dol->DolEqual_p((dol->vertices+i)->data,data))
        {
            return i;
        }
        i++;
    }
    return -1;
}
//��������DOL_AddVertice
//�� �ܣ���ʮ�����������һ�����㣬
//      ����ӻ�ʱ�Զ����ã��綥�㲻�������Զ����ô˺���
//�� �룺Ŀ��ʮ������dol������ָ��data�����ݴ洢��Сdsize
//�� �����ɹ������¶����ڶ�����е����������򷵻�-1
int DOL_AddVertice(DigraphOrthogonalList* dol,
                   void* data,int dsize)
{
    if(dol == NULL || data == NULL || dsize < 1)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return -1;
    }
    if(dol->pos >= dol->size-1)//������Ŀ����
    {
        DOL_LastError = DOL_ERROR_FULLLIST;
        return -1;
    }
    if(DOL_LocateVer(dol,data)!=-1)//�����Ѵ���
    {
        DOL_LastError = DOL_ERROR_VEREXITS;
        return -1;
    }
    dol->pos++;//��ǰ����λ�ü�1
    (dol->vertices+dol->pos)->data =
            malloc(dsize);
    //�ڴ�������

    if((dol->vertices+dol->pos)->data == NULL)
    {
        DOL_LastError = DOL_ERROR_MALLOCFAILD;
        return -1;
    }
    //��ʼ������
    memcpy((dol->vertices+dol->pos)->data,data,dsize);
    (dol->vertices+dol->pos)->firstIn = NULL;
    (dol->vertices+dol->pos)->firstOut = NULL;
    printf("����Ӷ��㣺%s\n",dol->DolPrint(data));
    return dol->pos;
}
//��������DOL_AddArc
//�� �ܣ���ʮ�����������һ����
//�� �룺Ŀ��ʮ������dol��β��������ָ��v1��ͷ��������ָ��v2��
//          β�������ݴ�Сsize1��ͷ�������ݴ�Сsize2
//�� �����ɹ�����1�����򷵻�0
int DOL_AddArc(DigraphOrthogonalList* dol,void* v1,void* v2,
               int size1,int size2)
{
    //��������
    if(dol == NULL || v1 == NULL || v2 == NULL
            || size1 < 1 || size2 < 1)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return 0;
    }
    //��������ڶ�����е�����
    int i = DOL_LocateVer(dol,v1);
    int j = DOL_LocateVer(dol,v2);
    //���������δ�ڶ�����У�����ӽ���������ӵ��������
    if(i == -1)
    {
        i = DOL_AddVertice(dol,v1,size1);
    }
    if(j == -1)
    {
        j = DOL_AddVertice(dol,v2,size2);
    }
    if(i == -1 || j == -1)//��Ӷ���ʧ��
    {
        return 0;
    }
    DOL_ARC* arc = (dol->vertices+i)->firstOut;
    while (arc!=NULL) {
        if(arc->head == j)
        {
            DOL_LastError = DOL_ERROR_ARCEXITS;
            return 0;
        }
        arc = arc->tlink;
    }
    DOL_ARC* new_arc = (DOL_ARC*)malloc(sizeof(DOL_ARC));
    if(new_arc == NULL)
    {
        DOL_LastError = DOL_ERROR_MALLOCFAILD;
        return 0;
    }
    //��ӻ���Ϣ
    new_arc->head = j;
    new_arc->tail = i;
    new_arc->hlink = (dol->vertices+j)->firstIn;
    new_arc->tlink = (dol->vertices+i)->firstOut;
    //�޸���Ӧ�������Ϣ
    (dol->vertices+i)->firstOut = new_arc;
    (dol->vertices+j)->firstIn = new_arc;
    printf("����ӻ���%s - ",dol->DolPrint(v1));
    printf("%s\n",dol->DolPrint(v2));
    return 1;
}
//��������DOL_DGA_p
//�� �ܣ��������������ж�����ͼdol�Ƿ�Ϊ�����޻�ͼ
//�� �룺Ŀ��ʮ������dol
//�� �����������޻�ͼ����1�����򷵻�0,���󷵻�-1
int DOL_DGA_p(DigraphOrthogonalList* dol)
{
    if(dol == NULL)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return -1;
    }
    if(dol->pos == -1)
    {
        DOL_LastError = DOL_ERROR_EMPTYLIST;
        return -1;
    }
    int id[dol->pos+1];//��ȱ�
    int i = 0;
    DOL_ARC* arc;//��ָ��
    //ͳ��ÿ����������
    while(i<=dol->pos)
    {
       id[i] = 0;//��ʼΪ��
       arc = (dol->vertices+i)->firstIn;
       while(arc!=NULL)
       {
           id[i]++;//ÿ��һ��ͷ����Ϊ�˶���Ļ����˶������ȼ�1
           arc = arc->hlink;
       }
       i++;
    }
    int index;
    Stack *s;
    if(!StackInit(&s,TYPE_INT,dol->pos+1))
    {
        DOL_LastError = DOL_ERROR_STACK;
        return -1;
    }

    int counter = 0;//ͳ�Ƴ�ջ�������

    //�����Ϊ0�Ķ�����ջ
    i = 0;
    while(i<=dol->pos)
    {
        if(id[i]==0)
        {
            push(s,&i);
            id[i] = -1;
        }
        i++;
    }
    //���������㷨
    while(touch(s)!=NULL)
    {

        index = *(int*)pop(s);
        counter++;
        //�ҵ���indexΪǰ���Ķ��㣬������ȼ�һ
        arc = (dol->vertices+index)->firstOut;
        while(arc!=NULL)
        {
            id[arc->head]--;
            arc = arc->tlink;
        }
        //�����Ϊ0�Ķ�����ջ
        i = 0;
        while(i<=dol->pos)
        {
            if(id[i]==0)
            {
                push(s,&i);
                id[i] = -1;
            }
            i++;
        }
    }
    //�������������㷨�������ջ�����ڶ��������������޻�ͼ
    if(counter == dol->pos+1)
    {
        return 1;
    }
    else
        return 0;
}
const char* DOL_getLastError()
{
    switch (DOL_LastError) {
    case DOL_ERROR_INVALIDPARA:
        return str_dol_error_invalidpara;
        break;
    case DOL_ERROR_EMPTYLIST:
        return str_dol_error_emptylist;
        break;
    case DOL_ERROR_FULLLIST:
        return str_dol_error_fulllist;
        break;
    case DOL_ERROR_STACK:
        return str_dol_error_stack;
        break;
    case DOL_ERROR_MALLOCFAILD:
        return str_dol_error_mallocfaild;
        break;
    case DOL_ERROR_VEREXITS:
        return str_dol_error_verexits;
        break;
    case DOL_ERROR_ARCEXITS:
        return str_dol_error_arcexits;
        break;
    default:
        return str_dol_error_null;
        break;
    }
    return NULL;
}
//��������DOL_Clear
//�� �ܣ����ʮ������
//�� �룺Ŀ��ʮ������
//�� �����ɹ�����1�����򷵻�0
int DOL_Clear(DigraphOrthogonalList* dol)
{
    if(dol == NULL)//��������
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return 0;
    }
    if(dol->pos == -1)//������
    {
        DOL_LastError = DOL_ERROR_EMPTYLIST;
        return 1;
    }
    int i = 0;
    DOL_ARC* arc;
    DOL_ARC* garbage;//������
    DOL_VERTICE* ver;
    while(i<=dol->pos)//�������ж���
    {
        ver = dol->vertices+i;
        free(ver->data);//������������
        ver->data = NULL;
        arc = ver->firstOut;
        while (arc!=NULL) {//���������Ըö���Ϊβ����Ļ�
            garbage = arc;
            arc = arc->tlink;
            free(garbage);//�����ڴ�
        }
        //ָ��NULL
        ver->firstIn = NULL;
        ver->firstOut = NULL;
        i++;
    }
    dol->pos = -1;//��ǰ����ָ��-1
    return 1;
}
