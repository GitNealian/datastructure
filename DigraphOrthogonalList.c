#include "digraphorthogonallist.h"

const char* str_dol_error_invalidpara = "参数无效";
const char* str_dol_error_mallocfaild = "内存分配错误";
const char* str_dol_error_stack = "栈错误";
const char* str_dol_error_fulllist = "链表满错误";
const char* str_dol_error_emptylist = "链表空错误";
const char* str_dol_error_verexits = "顶点已存在";
const char* str_dol_error_arcexits = "弧已存在";
const char* str_dol_error_null = "没有错误";
enum dol_error{DOL_ERROR_INVALIDPARA,DOL_ERROR_MALLOCFAILD,
           DOL_ERROR_STACK,DOL_ERROR_FULLLIST,DOL_ERROR_EMPTYLIST,
           DOL_ERROR_VEREXITS,DOL_ERROR_ARCEXITS,DOL_ERROR_NULL}DOL_LastError;
//函数名：DOL_Init
//功 能：初始化十字链表
//输 入：指向十字链表指针的指针dol，有向图顶点个数size，
//       判断相等与否的函数指针DolEqual_p
//输 出：成功返回1，否则返回0
int DOL_Init(DigraphOrthogonalList** dol,int size,
               int (*DolEqual_p)(void*,void*),
             char* (*DolPrint)(void*))
{
    //参数错误
    if(dol == NULL || size < 1 || DolEqual_p == NULL)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return 0;
    }
    *dol = (DigraphOrthogonalList*)
            malloc(sizeof(DigraphOrthogonalList));
    if(*dol == NULL)//内存分配错误
    {
        DOL_LastError = DOL_ERROR_MALLOCFAILD;
        return 0;
    }
    //初始化十字链表
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
    //内容清零
    memset((*dol)->vertices,0,sizeof(DOL_VERTICE)*size);
    (*dol)->DolEqual_p = DolEqual_p;
    (*dol)->DolPrint = DolPrint;
    DOL_LastError = DOL_ERROR_NULL;
    return 1;
}
//函数名：DOL_LocateVer
//功 能：获取数据在十字链表顶点表中的索引
//输 入：目标十字链表dol，要查找的数据指针data
//输 出：数据在顶点表中返回其索引，否则返回-1
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
        //遍历比较，适合顶点不多的情况
        //如顶点数目大，可采用其它结构
        //本实验顶点较少
        if(dol->DolEqual_p((dol->vertices+i)->data,data))
        {
            return i;
        }
        i++;
    }
    return -1;
}
//函数名：DOL_AddVertice
//功 能：往十字链表中添加一个顶点，
//      在添加弧时自动调用，如顶点不存在则自动调用此函数
//输 入：目标十字链表dol，数据指针data，数据存储大小dsize
//输 出：成功返回新顶点在顶点表中的索引，否则返回-1
int DOL_AddVertice(DigraphOrthogonalList* dol,
                   void* data,int dsize)
{
    if(dol == NULL || data == NULL || dsize < 1)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return -1;
    }
    if(dol->pos >= dol->size-1)//顶点数目已满
    {
        DOL_LastError = DOL_ERROR_FULLLIST;
        return -1;
    }
    if(DOL_LocateVer(dol,data)!=-1)//顶点已存在
    {
        DOL_LastError = DOL_ERROR_VEREXITS;
        return -1;
    }
    dol->pos++;//当前顶点位置加1
    (dol->vertices+dol->pos)->data =
            malloc(dsize);
    //内存分配错误

    if((dol->vertices+dol->pos)->data == NULL)
    {
        DOL_LastError = DOL_ERROR_MALLOCFAILD;
        return -1;
    }
    //初始化顶点
    memcpy((dol->vertices+dol->pos)->data,data,dsize);
    (dol->vertices+dol->pos)->firstIn = NULL;
    (dol->vertices+dol->pos)->firstOut = NULL;
    printf("已添加顶点：%s\n",dol->DolPrint(data));
    return dol->pos;
}
//函数名：DOL_AddArc
//功 能：往十字链表中添加一条弧
//输 入：目标十字链表dol，尾顶点数据指针v1，头顶点数据指针v2，
//          尾顶点数据大小size1，头顶点数据大小size2
//输 出：成功返回1，否则返回0
int DOL_AddArc(DigraphOrthogonalList* dol,void* v1,void* v2,
               int size1,int size2)
{
    //参数错误
    if(dol == NULL || v1 == NULL || v2 == NULL
            || size1 < 1 || size2 < 1)
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return 0;
    }
    //获得数据在顶点表中的索引
    int i = DOL_LocateVer(dol,v1);
    int j = DOL_LocateVer(dol,v2);
    //如果有数据未在顶点表中，则添加将此数据添加到顶点表中
    if(i == -1)
    {
        i = DOL_AddVertice(dol,v1,size1);
    }
    if(j == -1)
    {
        j = DOL_AddVertice(dol,v2,size2);
    }
    if(i == -1 || j == -1)//添加顶点失败
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
    //添加弧信息
    new_arc->head = j;
    new_arc->tail = i;
    new_arc->hlink = (dol->vertices+j)->firstIn;
    new_arc->tlink = (dol->vertices+i)->firstOut;
    //修改相应顶点的信息
    (dol->vertices+i)->firstOut = new_arc;
    (dol->vertices+j)->firstIn = new_arc;
    printf("已添加弧：%s - ",dol->DolPrint(v1));
    printf("%s\n",dol->DolPrint(v2));
    return 1;
}
//函数名：DOL_DGA_p
//功 能：利用拓扑排序法判断有向图dol是否为有向无环图
//输 入：目标十字链表dol
//输 出：是有向无环图返回1，否则返回0,错误返回-1
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
    int id[dol->pos+1];//入度表
    int i = 0;
    DOL_ARC* arc;//弧指针
    //统计每个顶点的入度
    while(i<=dol->pos)
    {
       id[i] = 0;//初始为零
       arc = (dol->vertices+i)->firstIn;
       while(arc!=NULL)
       {
           id[i]++;//每有一个头顶点为此顶点的弧，此顶点的入度加1
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

    int counter = 0;//统计出栈顶点个数

    //将入度为0的顶点入栈
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
    //拓扑排序算法
    while(touch(s)!=NULL)
    {

        index = *(int*)pop(s);
        counter++;
        //找到以index为前驱的顶点，将其入度减一
        arc = (dol->vertices+index)->firstOut;
        while(arc!=NULL)
        {
            id[arc->head]--;
            arc = arc->tlink;
        }
        //将入度为0的顶点入栈
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
    //根据拓扑排序算法，如果出栈数等于顶点数则是有向无环图
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
//函数名：DOL_Clear
//功 能：清空十字链表
//输 入：目标十字链表
//输 出：成功返回1，否则返回0
int DOL_Clear(DigraphOrthogonalList* dol)
{
    if(dol == NULL)//参数错误
    {
        DOL_LastError = DOL_ERROR_INVALIDPARA;
        return 0;
    }
    if(dol->pos == -1)//空链表
    {
        DOL_LastError = DOL_ERROR_EMPTYLIST;
        return 1;
    }
    int i = 0;
    DOL_ARC* arc;
    DOL_ARC* garbage;//待回收
    DOL_VERTICE* ver;
    while(i<=dol->pos)//遍历所有顶点
    {
        ver = dol->vertices+i;
        free(ver->data);//清理顶点数据项
        ver->data = NULL;
        arc = ver->firstOut;
        while (arc!=NULL) {//清理所有以该顶点为尾顶点的弧
            garbage = arc;
            arc = arc->tlink;
            free(garbage);//回收内存
        }
        //指向NULL
        ver->firstIn = NULL;
        ver->firstOut = NULL;
        i++;
    }
    dol->pos = -1;//当前索引指向-1
    return 1;
}
