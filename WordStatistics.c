#include "wordstatistics.h"

FILE* filep;//用于将单词统计表写到文件中
WST* merge_dest;//
//二叉树访问函数
void visit(void* word)
{
    if(filep!=NULL)
    {
        fprintf(filep,"%s : %d\n",((WST_WORD*)word)->word,((WST_WORD*)word)->times);
    }

}
//另一个访问函数
void merge(void* word)
{
    if(merge_dest!=NULL)
    {
        WST_AddWord(merge_dest,((WST_WORD*)word)->word,
                    ((WST_WORD*)word)->times);
    }
}

//用于二叉搜索树的排序函数
int sbt_sortFun(SBT_NODE* n1,SBT_NODE* n2)
{
    //节点中字符串的比较结果作为节点的比较结果
    return strcmp(((WST_WORD*)n1->data)->word,
                  ((WST_WORD*)n2->data)->word);
}
//函数名：WST_Init
//功能：初始化单词统计表
//输入：指向目标单词统计表的指针的指针wst，最大单词数max_word
//输出：成功返回1，否则返回0
int WST_Init(WST** wst,int max_word)
{
    if(wst == NULL || max_word < 1)//参数错误
    {
        return 0;
    }
    *wst = (WST*)malloc(sizeof(WST));
    if(*wst == NULL)//内存分配错误
    {
        return 0;
    }
    int i;
    //创建一个二叉树存储单词
    SBT* sbt;
    //二叉树初始化失败
    if(!SBT_Init(&sbt,sbt_sortFun,max_word))
    {
        free(*wst);
        return 0;
    }
    (*wst)->sbt = sbt;

    FirstTen* ft = (FirstTen*)malloc(sizeof(FirstTen));
    if(ft == NULL)//内存分配错误
    {
        free(*wst);
        return 0;
    }
    ft->word = (WST_WORD*)malloc(sizeof(WST_WORD));
    if(ft->word == NULL)//内存分配错误
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
    //创建一个双向链表用于维护出现次数前十的单词
    //为了便于操作first结点不用于存储
    for(i = 0; i < 10; i++)
    {
       ft  = (FirstTen*)malloc(sizeof(FirstTen));
       if(ft == NULL)//内存分配错误
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
       if(ft->word == NULL)//内存分配错误
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
//函数名：WST_InsertWord
//功能：往单词统计表中添加一个新单词
//输入：目标单词统计表wst，待添加的单词word
//输出：成功返回1，否则返回0
int WST_InsertWord(WST* wst,WST_WORD* word)
{
    //此函数只在本文件中调用，就不作参数检查了
    //在此调用中，word是堆中的变量，不是临时变量
    //所以可直接引用其指针
    if(!SBT_AddNode(wst->sbt,word))//二叉树添加错误
    {
        return 0;
    }
    return 1;
}
//函数名：WST_FirstTenUpdate
//功能：更新单词统计表的前十名
//输入：目标单词统计表wst，待更新的单词word
//输出：成功返回1，否则返回0
int WST_FirstTenUpdate(WST* wst,WST_WORD* word)
{
    if(wst == NULL || word == NULL)
    {
        return 0;
    }
    FirstTen* front = wst->first->next;

    while(front!=NULL)
    {
        //如果word已经在前十里面只需考察是否需要前移
        if(front->word->word!=NULL &&
                strcmp(front->word->word,word->word)==0)
        {
            FirstTen* pre = front->pre;
            //向前比较、更新
            while(pre!=wst->first
                  && (pre->word->times < front->word->times
                  || (pre->word->times == front->word->times
                  && strcmp(pre->word->word,front->word->word)>0)))
            {
                pre = pre->pre;
            }
            //此时pre为第一个不小于front的结点
            //如果pre就是front前一个的话就不需要移动了
            if(pre!=front->pre)
            {
                //如果front是最后一个
                //那它是不存在next结点的
                if(front == wst->last)
                {
                    front->pre->next = front->next;
                    wst->last = front->pre;
                }
                else
                {
                    front->pre->next = front->next;
                    //front不是最后一个
                    //那它的next结点的前一个结点指向front的
                    //前一个结点
                    front->next->pre = front->pre;
                }
                //上面将front从循环链表中删除
                //下面将front插入到pre后面
                front->next = pre->next;
                pre->next->pre = front;
                front->pre = pre;
                pre->next = front;
            }
            return 1;
        }
        front = front->next;
    }
    //下面处理word不在前十当中的情况
    front = wst->first->next;
    while(front!=NULL)
    {
        //如果word有进入前十的资格
        if(word->times>front->word->times ||(
                    front->word->times == word->times
                       && strcmp(word->word,front->word->word)<0))
        {
            //此时front是第一个小于word的结点
            FirstTen* new_ft = (FirstTen*)malloc(sizeof(FirstTen));
            if(new_ft == NULL)//内存分配错误
            {
                return 0;
            }
            new_ft->word = word;
            //将新结点插入到front之前
            new_ft->pre = front->pre;
            front->pre->next = new_ft;
            new_ft->next = front;
            front->pre = new_ft;
            //原来的最后一名退出前十链表
            FirstTen* garbage = wst->last;
            //原来的倒二成为最后
            wst->last = wst->last->pre;
            wst->last->next = NULL;
            free(garbage);//清理原来最后一名所占内存
            break;
        }
        front = front->next;
    }
    return 1;
}
//函数名：WST_AddWord
//功能：往单词统计表中添加一个单词项
//输入：目标单词统计表，要添加的单词字符串
//输出：成功返回1，否则返回0
int WST_AddWord(WST* wst, char* word, int times)
{
    if(wst == NULL || word == NULL)//参数错误
    {
        return 0;
    }
    SBT_NODE* node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(node == NULL)//内存分配错误
    {
        return 0;
    }
    int size = strlen(word)+1;

    WST_WORD* new_word = (WST_WORD*)malloc(sizeof(WST_WORD));
    if(new_word == NULL)//内存分配错误
    {
        free(node);
        return 0;
    }
    new_word->word = (char*)malloc(size);
    if(new_word->word == NULL)//内存分配错误
    {
        free(node);
        free(new_word);
        return 0;
    }
    memcpy(new_word->word,word,size);//复制数据到堆内存
    new_word->times = times;//新单词出现次数
    node->data = new_word;
    //上面创建了二叉树结点
    //由于之前写的SBT_Search需要传入SBT_NODE*，
    //（因为当时比较函数给出的接口的参数是两个SBT_NODE*）
    //所以此处需要创建二叉树结点用于查找
    //实际中单词已经存在于二叉树中的情况居多
    //而要创建结点的情况为少数，因此这样做增加了时间上的开销
    SBT_NODE* index = SBT_Search(wst->sbt,node);
    if(index!=NULL)//如果word已存在，word的次数加1
    {
        ((WST_WORD*)(index->data))->times += times;
        free(node);//结点已经存在那么node就不再需要了
        free(new_word);
    }
    else//否则插入此单词
    {
        index = node;
        WST_InsertWord(wst,new_word);
    }
    WST_FirstTenUpdate(wst,(WST_WORD*)index->data);//更新前十个word
    return 1;
}
//函数名：WST_ToFile
//功能：将单词统计结果保存到文件中
//输入：目标单词统计表wst，要保存到的文件指针file
//file要求具有写权限
//输出：成功返回1，否则返回0
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
//函数名：WST_LoadFromFile
//功能：从文件中读取并生成单词统计表
//输入：目标文件fd，目标单词统计表wst
//输出：成功返回1，否则返回0
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
//函数名：WST_Search
//功能：在单词统计表wst中查找单词word
//输入：目标单词统计表wst，要查找的单词字符串word
//输出：找到返回其WST_WORD指针，否则返回NULL
WST_WORD* WST_Search(WST* wst, char* word)
{

    if(wst == NULL || word == NULL)//参数错误
    {
        return 0;
    }
    SBT_NODE* node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(node == NULL)//内存分配错误
    {
        return 0;
    }

    WST_WORD* new_word = (WST_WORD*)malloc(sizeof(WST_WORD));
    if(new_word == NULL)//内存分配错误
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
//函数名：WST_Merge
//功能：将两个单词统计表整合到一起
//输入：目标单词统计表dest，源单词统计表src
//输出：1

int WST_Merge(WST* dest,WST* src)
{
    merge_dest = dest;
    SBT_Taversal_Inorder(src->sbt,merge);
    merge_dest = NULL;
    return 1;
}

//函数名：WST_Destory
//功能：销毁目标单词统计表
//输入：目标单词统计表
//输出：成功返回1，否则返回0
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
