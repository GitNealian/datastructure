#include "SortedBinaryTree.h"

const char* str_error_invalidpara = "参数无效";
const char* str_error_mallocfaild = "内存分配错误";
const char* str_error_stack = "栈错误";
const char* str_error_fulltree = "树满错误";
const char* str_error_emptytree = "树空错误";
const char* str_error_nodeexits = "结点已存在";
const char* str_error_null = "没有错误";
enum error{ERROR_INVALIDPARA,ERROR_MALLOCFAILD,
           ERROR_STACK,ERROR_FULLTREE,ERROR_EMPTYTREE,
           ERROR_NODEEXITS}SBT_LastError;
//函数名：SBT_Init
//功 能：初始化二叉排序树
//输 入：指向二叉排序树指针的指针sbt，比较函数的指针fun，最大长度max_len
//输 出：成功返回1，否则返回0
int SBT_Init(SBT** sbt, int(*fun)(SBT_NODE*,SBT_NODE*), int max_len)
{
    if(sbt == NULL||fun==NULL||max_len<1)//参数错误
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    *sbt = (SBT*)malloc(sizeof(SBT));

    if(*sbt == NULL)//内存分配错误
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        return 0;
    }
    //初始化二叉树
    (*sbt)->len = 0;
    (*sbt)->max_len = max_len;
    (*sbt)->root = NULL;
    (*sbt)->sortFun = fun;
    return 1;
}

//函数名：SBT_Insert
//功 能：将数据插入到排序二叉树中
//输 入：目标二叉树sbt，数据指针d，数据长度length
//输 出：成功返回1，否则返回0
int SBT_AddNode(SBT* sbt, void* d)
{
    if(sbt == NULL || d == NULL)//参数错误
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(sbt->len>=sbt->max_len)
    {
        SBT_LastError = ERROR_FULLTREE;
        return 0;
    }
    //产生一个新结点
    SBT_NODE* new_node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(new_node == NULL)//内存分配错误
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        return 0;
    }
    new_node->data = d;
    new_node->leftChild = NULL;
    new_node->rightChild = NULL;
    if(sbt->root == NULL)//如果二叉树为空
    {
        sbt->root = new_node;//将新结点作为根结点
    }
    else
    {
        SBT_NODE* node = sbt->root;
        int cmp;
        while (1)
        {
            cmp = sbt->sortFun(new_node,node);
            //如果新结点小于此结点，将新结点插入到此结点的左子树
            if(cmp<0)
            {
                if(node->leftChild==NULL)
                {
                    node->leftChild = new_node;
                    break;
                }
                else
                {
                    node = node->leftChild;
                }
            }
            //如果新结点大于些结点，将新结点插入到此结点的右子树
            else if(cmp>0)
            {
                if(node->rightChild==NULL)
                {
                    node->rightChild = new_node;
                    break;
                }
                else
                {
                    node = node->rightChild;
                }
            }
            //如果新结点在树中已经存在则放弃插入
            else
            {
                SBT_LastError = ERROR_NODEEXITS;
                free(new_node->data);//清理内存
                free(new_node);
                return 0;
            }
        }
    }
    sbt->len++;//长度加1
    return 1;
}


//函数名：SBT_Insert
//功 能：将数据插入到排序二叉树中
//输 入：目标二叉树sbt，数据指针d，数据长度length
//输 出：成功返回1，否则返回0
int SBT_Insert(SBT* sbt, void* d, int size)
{
    if(sbt == NULL || d == NULL || size<1)//参数错误
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(sbt->len>=sbt->max_len)
    {
        SBT_LastError = ERROR_FULLTREE;
        return 0;
    }
    //产生一个新结点
    SBT_NODE* new_node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(new_node == NULL)//内存分配错误
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        return 0;
    }
    //为数据域分配内存
    new_node->data = malloc(size);
    if(new_node->data == NULL)//内存分配错误
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        free(new_node);//清理内存
        return 0;
    }
    memcpy(new_node->data, d, size);
    new_node->leftChild = NULL;
    new_node->rightChild = NULL;
    if(sbt->root == NULL)//如果二叉树为空
    {
        sbt->root = new_node;//将新结点作为根结点
    }
    else
    {
        SBT_NODE* node = sbt->root;
        int cmp;
        while (1)
        {
            cmp = sbt->sortFun(new_node,node);
            //如果新结点小于此结点，将新结点插入到此结点的左子树
            if(cmp<0)
            {
                if(node->leftChild==NULL)
                {
                    node->leftChild = new_node;
                    break;
                }
                else
                {
                    node = node->leftChild;
                }
            }
            //如果新结点大于些结点，将新结点插入到此结点的右子树
            else if(cmp>0)
            {
                if(node->rightChild==NULL)
                {
                    node->rightChild = new_node;
                    break;
                }
                else
                {
                    node = node->rightChild;
                }
            }
            //如果新结点在树中已经存在则放弃插入
            else
            {
                SBT_LastError = ERROR_NODEEXITS;
                free(new_node->data);//清理内存
                free(new_node);
                return 0;
            }
        }
    }
    sbt->len++;//长度加1
    return 1;
}
//函数名：SBT_Taversal_Inorder
//功 能：中序遍历排序树
//输 入：目标二叉树sbt，访问函数指针visit
int SBT_Taversal_Inorder(SBT* sbt,void(*visit)(void*))
{
    if(sbt==NULL||visit==NULL)//参数错误
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    Stack* s;
    if(!StackInit(&s, TYPE_POINTER, sbt->len))//初始化栈
    {
        SBT_LastError = ERROR_STACK;
        return 0;
    }
    SBT_NODE* node = sbt->root;
    if(node == NULL)//空树
    {
        SBT_LastError = ERROR_EMPTYTREE;
        return 0;
    }
    push(s,&node);//根结点入栈
    while (!EmptyStack_p(s))//当栈不为空时
    {
        node = *(SBT_NODE**)touch(s);
        //如果栈顶元素不是NULL说明下层子树仍未访问，需要先访问下层子树
        if(node!=NULL)
        {
            //访问下层子树时先访问左子树
            while(node->leftChild!=NULL)
            {
                push(s,&(node->leftChild));//将左子树入栈
                node = node->leftChild;
            }
        }
        //如果栈顶元素为NULL，说明下层子树访问完毕 。
        else
        {
            pop(s);//将栈顶的NULL出栈
        }
        if(!EmptyStack_p(s))//当栈不为空时
        {
            //栈中最多只会有一个NULL元素，且此前已出栈，
            //所以node一定不为NULL
            node = *(SBT_NODE**)pop(s);
            visit(node->data);//访问此结点
            //将右子树入栈，如果右子树为空说明下层子树访问完毕
            push(s,&(node->rightChild));
        }

    }
    return 1;
}
//函数名：SBT_Search
//功 能：查找二叉树中的某个结点
//输 入：目标二叉树sbt，要查找的结点node
//输 出：若结点存在于二叉树中返回结点索引，否则返回NULL
SBT_NODE* SBT_Search(SBT* sbt, SBT_NODE* node)
{
    SBT_NODE* n = sbt->root;
    int rst;
    while (n!=NULL) {
        rst = sbt->sortFun(n,node);
        if(rst>0)
        {
            n = n->leftChild;
        }
        else if(rst<0)
        {
            n = n->rightChild;
        }
        else {
            break;
        }
    }
    return n;
}

//函数名：SBT_Clear
//功 能：清空二叉树
//输 入：目标二叉树sbt
//输 出：成功返回1，否则返回0
int SBT_Clear(SBT* sbt)
{
    if(sbt==NULL)//参数错误
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(sbt->root==NULL)//空二叉树
    {
        SBT_LastError = ERROR_EMPTYTREE;
        return 1;
    }
    Stack* s;
    if(!StackInit(&s,TYPE_POINTER,sbt->len))//初始化栈
    {
        SBT_LastError = ERROR_STACK;
        return 0;
    }
    SBT_NODE* node;
    push(s,&(sbt->root));//根结点入栈
    while (!EmptyStack_p(s))//栈不为空时，前序遍历二叉树
    {
        node = *(SBT_NODE**)pop(s);
        if(node->rightChild!=NULL)
        {
            push(s,&(node->rightChild));
        }
        if(node->leftChild!=NULL)
        {
            push(s,&(node->leftChild));
        }
        free(node->data);//清理内存
        free(node);
    }
    sbt->len = 0;//长度为0
    sbt->root = NULL;//根结点为NULL
    return 1;
}
//函数名：SBT_Destory
//功 能：清除二叉树
//输 入：指向目标二叉树指针的指针
//输 出：成功返回1，否则返回0
int SBT_Destory(SBT** sbt)
{
    if(sbt == NULL || *sbt==NULL)//参数错误
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(!SBT_Clear(*sbt))//清空二叉树
    {
        return 0;
    }
    free(*sbt);//清理内存
    *sbt = NULL;//二叉树指针指向NULL
    return 1;
}
const char* SBT_getLastError()
{
    switch (SBT_LastError) {
    case ERROR_INVALIDPARA:
        return str_error_invalidpara;
        break;
    case ERROR_EMPTYTREE:
        return str_error_emptytree;
        break;
    case ERROR_FULLTREE:
        return str_error_fulltree;
        break;
    case ERROR_STACK:
        return str_error_stack;
        break;
    case ERROR_MALLOCFAILD:
        return str_error_mallocfaild;
        break;
    case ERROR_NODEEXITS:
        return str_error_nodeexits;
        break;
    default:
        return str_error_null;
        break;
    }
    return NULL;
}
