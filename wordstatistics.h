#ifndef WORDSTATISTICS_H
#define WORDSTATISTICS_H
#include "SortedBinaryTree.h"
typedef struct WST_WORD
{
    char* word;//单词字符串
    int times;//单词出现次数
}WST_WORD;
typedef struct FirstTen//双向链表的结点
{
    WST_WORD* word;
    struct FirstTen* pre;
    struct FirstTen* next;
}FirstTen;
typedef struct WST
{
    SBT* sbt;//二叉树
    FirstTen* first;//双向链表的头结点
    FirstTen* last;//双向链表的尾结点
}WST;


int WST_Init(WST** wst, int max_word);
int WST_AddWord(WST* wst, char* word, int times);
int WST_ToFile(WST* wst, FILE* file);
int WST_Destory(WST* wst);
WST_WORD* WST_Search(WST* wst, char* word);
int WST_LoadFromFile(FILE* fd,WST* wst);
int WST_Merge(WST* dist, WST* src);
#endif // WORDSTATISTICS_H
