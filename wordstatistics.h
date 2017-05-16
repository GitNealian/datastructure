#ifndef WORDSTATISTICS_H
#define WORDSTATISTICS_H
#include "SortedBinaryTree.h"
typedef struct WST_WORD
{
    char* word;//�����ַ���
    int times;//���ʳ��ִ���
}WST_WORD;
typedef struct FirstTen//˫������Ľ��
{
    WST_WORD* word;
    struct FirstTen* pre;
    struct FirstTen* next;
}FirstTen;
typedef struct WST
{
    SBT* sbt;//������
    FirstTen* first;//˫�������ͷ���
    FirstTen* last;//˫�������β���
}WST;


int WST_Init(WST** wst, int max_word);
int WST_AddWord(WST* wst, char* word, int times);
int WST_ToFile(WST* wst, FILE* file);
int WST_Destory(WST* wst);
WST_WORD* WST_Search(WST* wst, char* word);
int WST_LoadFromFile(FILE* fd,WST* wst);
int WST_Merge(WST* dist, WST* src);
#endif // WORDSTATISTICS_H
