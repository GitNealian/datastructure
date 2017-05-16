#ifndef DIGRAPHORTHOGONALLIST_H
#define DIGRAPHORTHOGONALLIST_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Stack.h"
typedef struct DOL_ARC
{
    int head;
    int tail;
    struct DOL_ARC* hlink;
    struct DOL_ARC* tlink;
}DOL_ARC;
typedef struct DOL_VERTICE
{
    void* data;
    DOL_ARC* firstIn;
    DOL_ARC* firstOut;
}DOL_VERTICE;

typedef struct DOL
{
    int size;
    int pos;
    DOL_VERTICE* vertices;
    int (*DolEqual_p)(void*,void*);
    char* (*DolPrint)(void*);
}DigraphOrthogonalList;
int DOL_Init(DigraphOrthogonalList** dol,int size,
               int (*DolEqual_p)(void*,void*),char*(*DOlPrint)(void*));
int DOL_LocateVer(DigraphOrthogonalList* dol,
                  void* data);
int DOL_AddVertice(DigraphOrthogonalList* dol,
                   void* data,int dsize);
int DOL_AddArc(DigraphOrthogonalList* dol,void* v1,void* v2,
               int size1,int size2);
int DOL_DGA_p(DigraphOrthogonalList* dol);
int DOL_Clear(DigraphOrthogonalList* dol);
const char* DOL_getLastError();
#endif // DIGRAPHORTHOGONALLIST_H
