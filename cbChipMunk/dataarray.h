#pragma once
#include "common.h"
//#ifndef _DATA_ARRAY_H_
//#define _DATA_ARRAY_H_
struct Variable_;
typedef struct Variable_ Variable;

typedef struct DataArray_
{
	char *mData;
	int mSize;
	Variable *mVariable;
} DataArray;

DataArray *daAlloc(void);
DataArray *daNew(char *data,int size);
DataArray *daNewNull(void);
void daFree(DataArray *a);
void daInit(DataArray *a,char *data,int size);
void daInitNull(DataArray *a);
void daDestroy(DataArray *a);
//#endif
