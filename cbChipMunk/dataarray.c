#include "dataarray.h"
#include "cbchipmunk.h"
#include "variablehandler.h"

DataArray *daAlloc(void)
{
	return (DataArray*)malloc(sizeof(DataArray));
}

void daInit(DataArray *a,char *data,int size)
{
	a->mData = data;
	a->mSize = size;
	a->mVariable = 0;
}

void daInitNull(DataArray *a)
{
	a->mData = 0;
	a->mSize = 0;
	a->mVariable = 0;
}

DataArray *daNew(char *data,int size)
{
	DataArray *a = daAlloc();
	daInit(a,data,size);
	return a;
}


DataArray *daNewNull(void)
{
	DataArray *a = daAlloc();
	daInitNull(a);
	return a;
}

void daFree(DataArray *a)
{
	daDestroy(a);
	free(a);
}

void daDestroy(DataArray *a)
{
	free(a->mData);
}

/*

__declspec( dllexport ) void dataarraynew( const void * _in, int in_size, void * _out, int out_sz )
{
	uchar mode;
	Variable *var;
	DataArray *dataArray = daNewNull();
	dataArray->mSize = PEEKINT(INPUT_MEMBLOCK,0);

	//Varataan muistia taulukkoa varten
	dataArray->mData = (char*)malloc(dataArray->mSize);

	mode = PEEKBYTE(INPUT_MEMBLOCK,8);
	switch (mode)
	{
	case 0: //Täytettään nollalla
		memset(dataArray->mData,0,dataArray->mSize);
		break;
	case 1://Data mukana...
		memcpy(dataArray->mData,((char*)INPUT_MEMBLOCK)+9,dataArray->mSize);
		break;
	default:
		MessageBoxA(NULL,"Invalid data-array initialize mode.","Error",MB_OK);
		exit(0);
	}

	var = vhAddDataArray(&mVariableHandler,dataArray);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,4);
	
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}
*/