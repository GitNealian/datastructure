#include "SortedBinaryTree.h"

const char* str_error_invalidpara = "������Ч";
const char* str_error_mallocfaild = "�ڴ�������";
const char* str_error_stack = "ջ����";
const char* str_error_fulltree = "��������";
const char* str_error_emptytree = "���մ���";
const char* str_error_nodeexits = "����Ѵ���";
const char* str_error_null = "û�д���";
enum error{ERROR_INVALIDPARA,ERROR_MALLOCFAILD,
           ERROR_STACK,ERROR_FULLTREE,ERROR_EMPTYTREE,
           ERROR_NODEEXITS}SBT_LastError;
//��������SBT_Init
//�� �ܣ���ʼ������������
//�� �룺ָ�����������ָ���ָ��sbt���ȽϺ�����ָ��fun����󳤶�max_len
//�� �����ɹ�����1�����򷵻�0
int SBT_Init(SBT** sbt, int(*fun)(SBT_NODE*,SBT_NODE*), int max_len)
{
    if(sbt == NULL||fun==NULL||max_len<1)//��������
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    *sbt = (SBT*)malloc(sizeof(SBT));

    if(*sbt == NULL)//�ڴ�������
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        return 0;
    }
    //��ʼ��������
    (*sbt)->len = 0;
    (*sbt)->max_len = max_len;
    (*sbt)->root = NULL;
    (*sbt)->sortFun = fun;
    return 1;
}

//��������SBT_Insert
//�� �ܣ������ݲ��뵽�����������
//�� �룺Ŀ�������sbt������ָ��d�����ݳ���length
//�� �����ɹ�����1�����򷵻�0
int SBT_AddNode(SBT* sbt, void* d)
{
    if(sbt == NULL || d == NULL)//��������
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(sbt->len>=sbt->max_len)
    {
        SBT_LastError = ERROR_FULLTREE;
        return 0;
    }
    //����һ���½��
    SBT_NODE* new_node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(new_node == NULL)//�ڴ�������
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        return 0;
    }
    new_node->data = d;
    new_node->leftChild = NULL;
    new_node->rightChild = NULL;
    if(sbt->root == NULL)//���������Ϊ��
    {
        sbt->root = new_node;//���½����Ϊ�����
    }
    else
    {
        SBT_NODE* node = sbt->root;
        int cmp;
        while (1)
        {
            cmp = sbt->sortFun(new_node,node);
            //����½��С�ڴ˽�㣬���½����뵽�˽���������
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
            //����½�����Щ��㣬���½����뵽�˽���������
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
            //����½���������Ѿ��������������
            else
            {
                SBT_LastError = ERROR_NODEEXITS;
                free(new_node->data);//�����ڴ�
                free(new_node);
                return 0;
            }
        }
    }
    sbt->len++;//���ȼ�1
    return 1;
}


//��������SBT_Insert
//�� �ܣ������ݲ��뵽�����������
//�� �룺Ŀ�������sbt������ָ��d�����ݳ���length
//�� �����ɹ�����1�����򷵻�0
int SBT_Insert(SBT* sbt, void* d, int size)
{
    if(sbt == NULL || d == NULL || size<1)//��������
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(sbt->len>=sbt->max_len)
    {
        SBT_LastError = ERROR_FULLTREE;
        return 0;
    }
    //����һ���½��
    SBT_NODE* new_node = (SBT_NODE*)malloc(sizeof(SBT_NODE));
    if(new_node == NULL)//�ڴ�������
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        return 0;
    }
    //Ϊ����������ڴ�
    new_node->data = malloc(size);
    if(new_node->data == NULL)//�ڴ�������
    {
        SBT_LastError = ERROR_MALLOCFAILD;
        free(new_node);//�����ڴ�
        return 0;
    }
    memcpy(new_node->data, d, size);
    new_node->leftChild = NULL;
    new_node->rightChild = NULL;
    if(sbt->root == NULL)//���������Ϊ��
    {
        sbt->root = new_node;//���½����Ϊ�����
    }
    else
    {
        SBT_NODE* node = sbt->root;
        int cmp;
        while (1)
        {
            cmp = sbt->sortFun(new_node,node);
            //����½��С�ڴ˽�㣬���½����뵽�˽���������
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
            //����½�����Щ��㣬���½����뵽�˽���������
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
            //����½���������Ѿ��������������
            else
            {
                SBT_LastError = ERROR_NODEEXITS;
                free(new_node->data);//�����ڴ�
                free(new_node);
                return 0;
            }
        }
    }
    sbt->len++;//���ȼ�1
    return 1;
}
//��������SBT_Taversal_Inorder
//�� �ܣ��������������
//�� �룺Ŀ�������sbt�����ʺ���ָ��visit
int SBT_Taversal_Inorder(SBT* sbt,void(*visit)(void*))
{
    if(sbt==NULL||visit==NULL)//��������
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    Stack* s;
    if(!StackInit(&s, TYPE_POINTER, sbt->len))//��ʼ��ջ
    {
        SBT_LastError = ERROR_STACK;
        return 0;
    }
    SBT_NODE* node = sbt->root;
    if(node == NULL)//����
    {
        SBT_LastError = ERROR_EMPTYTREE;
        return 0;
    }
    push(s,&node);//�������ջ
    while (!EmptyStack_p(s))//��ջ��Ϊ��ʱ
    {
        node = *(SBT_NODE**)touch(s);
        //���ջ��Ԫ�ز���NULL˵���²�������δ���ʣ���Ҫ�ȷ����²�����
        if(node!=NULL)
        {
            //�����²�����ʱ�ȷ���������
            while(node->leftChild!=NULL)
            {
                push(s,&(node->leftChild));//����������ջ
                node = node->leftChild;
            }
        }
        //���ջ��Ԫ��ΪNULL��˵���²������������ ��
        else
        {
            pop(s);//��ջ����NULL��ջ
        }
        if(!EmptyStack_p(s))//��ջ��Ϊ��ʱ
        {
            //ջ�����ֻ����һ��NULLԪ�أ��Ҵ�ǰ�ѳ�ջ��
            //����nodeһ����ΪNULL
            node = *(SBT_NODE**)pop(s);
            visit(node->data);//���ʴ˽��
            //����������ջ�����������Ϊ��˵���²������������
            push(s,&(node->rightChild));
        }

    }
    return 1;
}
//��������SBT_Search
//�� �ܣ����Ҷ������е�ĳ�����
//�� �룺Ŀ�������sbt��Ҫ���ҵĽ��node
//�� �������������ڶ������з��ؽ�����������򷵻�NULL
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

//��������SBT_Clear
//�� �ܣ���ն�����
//�� �룺Ŀ�������sbt
//�� �����ɹ�����1�����򷵻�0
int SBT_Clear(SBT* sbt)
{
    if(sbt==NULL)//��������
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(sbt->root==NULL)//�ն�����
    {
        SBT_LastError = ERROR_EMPTYTREE;
        return 1;
    }
    Stack* s;
    if(!StackInit(&s,TYPE_POINTER,sbt->len))//��ʼ��ջ
    {
        SBT_LastError = ERROR_STACK;
        return 0;
    }
    SBT_NODE* node;
    push(s,&(sbt->root));//�������ջ
    while (!EmptyStack_p(s))//ջ��Ϊ��ʱ��ǰ�����������
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
        free(node->data);//�����ڴ�
        free(node);
    }
    sbt->len = 0;//����Ϊ0
    sbt->root = NULL;//�����ΪNULL
    return 1;
}
//��������SBT_Destory
//�� �ܣ����������
//�� �룺ָ��Ŀ�������ָ���ָ��
//�� �����ɹ�����1�����򷵻�0
int SBT_Destory(SBT** sbt)
{
    if(sbt == NULL || *sbt==NULL)//��������
    {
        SBT_LastError = ERROR_INVALIDPARA;
        return 0;
    }
    if(!SBT_Clear(*sbt))//��ն�����
    {
        return 0;
    }
    free(*sbt);//�����ڴ�
    *sbt = NULL;//������ָ��ָ��NULL
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
