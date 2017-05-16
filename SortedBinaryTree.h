#ifndef SORTEDBINARYTREE_H_INCLUDED
#define SORTEDBINARYTREE_H_INCLUDED

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Stack.h"
typedef struct SBT_NODE
{
    void* data;
    struct SBT_NODE* leftChild;
    struct SBT_NODE* rightChild;
}SBT_NODE;
typedef struct SBT
{
    SBT_NODE* root;
    int (*sortFun)(SBT_NODE*, SBT_NODE*);
    int max_len;
    int len;
}SBT;

int SBT_Init(SBT** sbt, int(*fun)(SBT_NODE*,SBT_NODE*),int max_len);
int SBT_Insert(SBT* sbt, void* d, int size);
int SBT_AddNode(SBT* sbt, void* d);
int SBT_Taversal_Inorder(SBT* sbt,void(*visit)(void*));
int SBT_Clear(SBT* sbt);
SBT_NODE* SBT_Search(SBT* sbt, SBT_NODE* node);
int SBT_Destory(SBT** sbt);
const char* SBT_getLastError();
#endif // SORTEDBINARYTREE_H_INCLUDED
