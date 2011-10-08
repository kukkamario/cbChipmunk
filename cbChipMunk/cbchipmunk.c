
#include "cbchipmunk.h"

#include "variablehandler.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winbase.h>
#include <math.h>
#include "dataarray.h"





cpSpace mSpace;
VariableHandler mVariableHandler;
cpVect mWind;

static HANDLE mThread = NULL;
static DWORD mThreadID = 0;
static float mTimeStep = 0;
State mState = Unloaded;
static unsigned char mAutoClearForces = TRUE;
static float mUpdateTime = 0;



cpVect PEEKVECT(const void *mem,int p)
{
	cpVect ret;
	ret.x = *((float*)(((char*)mem)+p));
	ret.y = *((float*)(((char*)mem)+p+4));
	return ret;
}

void POKEVECT(void *mem,int p,const cpVect vec)
{
	POKEFLOAT(mem,p,vec.x);
	POKEFLOAT(mem,p+4,vec.y);
}


float degToRad(float deg)
{
	return deg*(float)M_PI/180.0f;
}

float radToDeg(float rad)
{
	return rad*180.0f/(float)M_PI;
}




void freeDll(void)
{
	
	int i;
	Variable *var;


	//Poistettaan kaikki muuttujat
	for (i = 1; i != mVariableHandler.mSize;i++)
	{
		var = mVariableHandler.mPtrArray[i];
		if (var != NULL)
		{
			switch(var->mType)
			{
			case VarTypeBody:
				cpBodyFree((cpBody*)var->mPtr);break;
			case VarTypeShape:
				cpShapeFree((cpShape*)var->mPtr);break;
			case VarTypeConstraint:
				cpConstraintFree((cpConstraint*)var->mPtr);break;
			case VarTypeDataArray:
				daFree((DataArray*)var->mPtr);break;
			}
		}
	}

	//Tuhotaan ne kaikki...
	vhDestroy(&mVariableHandler);
	cpSpaceDestroy(&mSpace);
	mState = Unloaded;
}

void initDll(void)
{
	Variable *mainStatic;
	cpSpaceInit(&mSpace);
	cpSpaceSetIterations(&mSpace,20);
	cpSpaceSetEnableContactGraph(&mSpace,cpFalse);
	cpSpaceSetSleepTimeThreshold(&mSpace,0.1f);
	vhInit(&mVariableHandler);
	mainStatic = vhAddBody(&mVariableHandler,cpSpaceGetStaticBody(&mSpace));
	mainStatic->mCBPtr = 0;
	mWind = cpv(0,0);
	mState = Loaded;
}


BOOL WINAPI DllMain(__in  HINSTANCE hinstDLL,__in  DWORD fdwReason,__in  LPVOID lpvReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	//case DLL_THREAD_ATTACH:
		if (mState == Unloaded) initDll();
		break;
	case DLL_PROCESS_DETACH:
	//case DLL_THREAD_DETACH:
		if (mState == Loaded) freeDll();
		break;
	}
	return TRUE;
}

__declspec( dllexport ) void test( const void * _in, int in_size, void * _out, int out_sz )
{
	char msg[80];
	sprintf(msg,"State:%i",mState);
	MessageBoxA(NULL,msg,"Test",MB_OK);
	sprintf(msg,"Size:%i",mVariableHandler.mSize);
	MessageBoxA(NULL,msg,"",MB_OK);
}



__declspec( dllexport ) void momentforcircle( const void * _in, int in_size, void * _out, int out_sz )
{
	POKEFLOAT(OUTPUT_MEMBLOCK,0,cpMomentForCircle(PEEKFLOAT(INPUT_MEMBLOCK,0),PEEKFLOAT(INPUT_MEMBLOCK,4),PEEKFLOAT(INPUT_MEMBLOCK,8),PEEKVECT(INPUT_MEMBLOCK,12)));
}
__declspec( dllexport ) void momentforpolygon( const void * _in, int in_size, void * _out, int out_sz )
{
	int size;
	int i;
	cpVect offset;
	float mass;
	cpVect *vertices;
	mass = PEEKFLOAT(INPUT_MEMBLOCK,0);
	offset = PEEKVECT(INPUT_MEMBLOCK,4);
	
	size = PEEKINT(INPUT_MEMBLOCK,12);
	vertices = (cpVect*)malloc(size*sizeof(cpVect));
	for (i = 0;i != size;i++)
	{
		vertices[i] = PEEKVECT(INPUT_MEMBLOCK,16+i*8);
	}
	POKEFLOAT(OUTPUT_MEMBLOCK,0,cpMomentForPoly(mass,size,vertices,offset));
	free(vertices);
}

__declspec( dllexport ) void momentforbox( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBB box;
	float w = PEEKFLOAT(INPUT_MEMBLOCK,4);
	float h = PEEKFLOAT(INPUT_MEMBLOCK,8);
	cpVect offset = PEEKVECT(INPUT_MEMBLOCK,12);
	box.l = offset.x-w*0.5f;
	box.r = box.l + w;
	box.t = offset.y + h*0.5f;
	box.b = box.t - h;
	POKEFLOAT(OUTPUT_MEMBLOCK,0,cpMomentForBox2(PEEKFLOAT(INPUT_MEMBLOCK,0),box));
}

__declspec( dllexport ) void momentforsegment( const void * _in, int in_size, void * _out, int out_sz )
{
	cpVect translated1 = PEEKVECT(INPUT_MEMBLOCK,4);
	cpVect translated2 = PEEKVECT(INPUT_MEMBLOCK,12);
	cpVect offset = PEEKVECT(INPUT_MEMBLOCK,20);

	translated1.x += offset.x;
	translated1.y += offset.y;
		
	translated2.x += offset.x;
	translated2.y += offset.y;

	POKEFLOAT(OUTPUT_MEMBLOCK,0,cpMomentForSegment(PEEKFLOAT(INPUT_MEMBLOCK,0),translated1,translated2));
}

__declspec( dllexport ) void areaforpolygon( const void * _in, int in_size, void * _out, int out_sz )
{
	int i;
	int size = PEEKINT(INPUT_MEMBLOCK,0);
	cpVect *vertices = (cpVect*)malloc(size*sizeof(cpVect));
	for (i = 0;i != size;i++)
	{
		vertices[i] = PEEKVECT(INPUT_MEMBLOCK,4+i*8);
	}
	POKEFLOAT(OUTPUT_MEMBLOCK,0,cpAreaForPoly(size,vertices));
	free(vertices);
}

__declspec( dllexport ) void recenterpolygon( const void * _in, int in_size, void * _out, int out_sz )
{
	int i;
	int size = PEEKINT(INPUT_MEMBLOCK,0);
	cpVect *vertices = (cpVect*)malloc(size*sizeof(cpVect));
	for (i = 0;i != size;i++)
	{
		vertices[i] = PEEKVECT(INPUT_MEMBLOCK,4+i*8);
	}
	cpRecenterPoly(size,vertices);
	for (i = 0;i != size;i++)
	{
		POKEVECT(OUTPUT_MEMBLOCK,i*8,vertices[i]);
	}
	free(vertices);
}


__declspec( dllexport ) void areaforsegment( const void * _in, int in_size, void * _out, int out_sz )
{

	POKEFLOAT(OUTPUT_MEMBLOCK,0,cpAreaForSegment(PEEKVECT(INPUT_MEMBLOCK,0),PEEKVECT(INPUT_MEMBLOCK,8),PEEKFLOAT(INPUT_MEMBLOCK,16)));
}

__declspec( dllexport ) void setiterations( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSpaceSetIterations(&mSpace,PEEKINT(INPUT_MEMBLOCK,0));
}


__declspec( dllexport ) void waitforupdate( const void * _in, int in_size, void * _out, int out_sz )
{
	//Tarkistetaan ett‰ ei kutsuta ennen updatea
	if (mThread)
	{
		//Odotetaan ett‰ fysiikat ovat p‰ivittyneet
		WaitForSingleObject(mThread,INFINITE);

		//Poistetaan s‰ie.
		CloseHandle(mThread);
		mThread = NULL;
	}

	//Kirjoitetaan p‰ivitysaika muistipalaan
	POKEFLOAT(OUTPUT_MEMBLOCK,0,mUpdateTime);
}


__declspec( dllexport ) void setgravity( const void * _in, int in_size, void * _out, int out_sz )
{
	//Asetetaan maailman painovoima
	cpSpaceSetGravity(&mSpace,PEEKVECT(INPUT_MEMBLOCK,0));
}





//Hakee yhden muuttujan uudet arvot 
__declspec( dllexport ) void pull( const void * _in, int in_size, void * _out, int out_sz )
{
	int index;
	Variable *var;
	cpBody *body;
	cpShape *shape;

	index = PEEKINT(INPUT_MEMBLOCK,0);
	var = vhGetVariable(&mVariableHandler,index);
	switch (var->mType)
	{
	case VarTypeBody:
		body = (cpBody*)var->mPtr;
		
		POKEFLOAT(OUTPUT_MEMBLOCK,0,radToDeg(cpBodyGetAngle(body)));
		POKEVECT(OUTPUT_MEMBLOCK,4,cpBodyGetPos(body));
		POKEFLOAT(OUTPUT_MEMBLOCK,12,radToDeg(cpBodyGetAngVel(body)));
		POKEVECT(OUTPUT_MEMBLOCK,16,cpBodyGetVel(body));
		break;

	case VarTypeShape:
		shape = (cpShape*)var->mPtr;
		POKEINT(OUTPUT_MEMBLOCK,0,VarTypeShape);
		
	default:
		MessageBoxA(NULL,"cpPull: Invalid variable type","cpChipmunk error",MB_OK);
		exit(0);
		break;
	}
}


__declspec( dllexport ) void setsleeptimethreshold( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSpaceSetSleepTimeThreshold(&mSpace,PEEKFLOAT(INPUT_MEMBLOCK,0));
}

__declspec( dllexport ) void setidlespeedthreshold( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSpaceSetIdleSpeedThreshold(&mSpace,PEEKFLOAT(INPUT_MEMBLOCK,0));
}







__declspec( dllexport ) void setautoclearforces( const void * _in, int in_size, void * _out, int out_sz )
{
	mAutoClearForces = PEEKBYTE(INPUT_MEMBLOCK,0);
}

__declspec( dllexport ) void setdamping( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSpaceSetDamping(&mSpace,PEEKFLOAT(INPUT_MEMBLOCK,0));
}

__declspec( dllexport ) void push( const void * _in, int in_size, void * _out, int out_sz )
{
	int index;
	Variable *var;
	cpBody *body;
	cpShape *shape;

	index = PEEKINT(INPUT_MEMBLOCK,0);
	var = vhGetVariable(&mVariableHandler,index);
	switch (var->mType)
	{
	case VarTypeBody:
		body = (cpBody*)var->mPtr;
		
		cpBodySetAngle(body,degToRad(PEEKFLOAT(INPUT_MEMBLOCK,4)));
		cpBodySetPos(body,PEEKVECT(INPUT_MEMBLOCK,8));
		cpBodySetAngVel(body,degToRad(PEEKFLOAT(INPUT_MEMBLOCK,16)));
		cpBodySetVel(body,PEEKVECT(INPUT_MEMBLOCK,20));
		break;

	case VarTypeShape:
		shape = (cpShape*)var->mPtr;
		cpShapeSetFriction(shape,PEEKFLOAT(INPUT_MEMBLOCK,4));
		cpShapeSetElasticity(shape,PEEKFLOAT(INPUT_MEMBLOCK,8));
		cpShapeSetLayers(shape,PEEKUINT(INPUT_MEMBLOCK,12));
		cpShapeSetGroup(shape,PEEKUINT(INPUT_MEMBLOCK,16));
		break;
	}
}




#define VARIABLE_LIST_SIZE 500
static Variable *mVariableList[VARIABLE_LIST_SIZE];
static int mVariableListIterator = 0;
void resetVariableList(void)
{
	mVariableListIterator = 0;
	memset(mVariableList,0,VARIABLE_LIST_SIZE*sizeof(Variable*));
}

void listShape(cpBody *body, cpShape *shape, void *data)
{
	Variable *var;
	var = (Variable*)shape->data;
	mVariableList[mVariableListIterator] = var;
	mVariableListIterator++;
}
void listConstraint(cpBody *body, cpConstraint *constraint, void *data)
{
	Variable *var;
	var = (Variable*)constraint->data;
	mVariableList[mVariableListIterator] = var;
	mVariableListIterator++;
}

__declspec( dllexport ) void deletevar( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body;
	int i;
	int index;
	Variable *var;
	Variable *mainVar = vhRemoveVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	
	if (mainVar->mType == VarTypeBody)
	{
		body = (cpBody*)mainVar->mPtr;
		resetVariableList();
		cpBodyEachShape(body,listShape,0);
		index = 0;
		for (i = 0;i != mVariableListIterator;i++)
		{
			var = mVariableList[i];
			
			POKEINT(OUTPUT_MEMBLOCK,index,var->mCBPtr);index += 4;
			
			cpSpaceRemoveShape(&mSpace,(cpShape*)var->mPtr);
			cpShapeFree((cpShape*)var->mPtr);
			//MessageBoxA(NULL,"ShapeRemoved2","Error",MB_OK);

			vhRemoveVariable(&mVariableHandler,var->mLocalHandle);
			varFree(var);
		}

		resetVariableList();
		cpBodyEachConstraint(body,listConstraint,0);
		for (i = 0;i != mVariableListIterator;i++)
		{
			var = mVariableList[i];
			POKEINT(OUTPUT_MEMBLOCK,index,var->mCBPtr);index+=4;
			//MessageBoxA(NULL,"Constraint removed2","Error",MB_OK);
			cpSpaceRemoveConstraint(&mSpace,(cpConstraint*)var->mPtr);
			cpConstraintFree((cpConstraint*)var->mPtr);
			vhRemoveVariable(&mVariableHandler,var->mLocalHandle);
			varFree(var);
		}
		POKEINT(OUTPUT_MEMBLOCK,index,0);//End point
		//MessageBoxA(NULL,"BodyRemoved","Error",MB_OK);
		cpSpaceRemoveBody(&mSpace,body);
		cpBodyFree(body);
		varFree(mainVar);
	}
	else if (mainVar->mType == VarTypeShape)
	{
		cpSpaceRemoveShape(&mSpace,(cpShape*)mainVar->mPtr);
		cpShapeFree((cpShape*)mainVar->mPtr);
		//MessageBoxA(NULL,"ShapeRemoved","Error",MB_OK);
		varFree(mainVar);
		POKEINT(OUTPUT_MEMBLOCK,0,0);
	}
	else if (mainVar->mType == VarTypeConstraint)
	{
		cpSpaceRemoveConstraint(&mSpace,(cpConstraint*)mainVar->mPtr);
		cpConstraintFree((cpConstraint*)mainVar->mPtr);
		varFree(mainVar);

		POKEINT(OUTPUT_MEMBLOCK,0,0);
	}
	else if (mainVar->mType == VarTypeDataArray)
	{
		daFree((DataArray*)mainVar->mPtr);
		varFree(mainVar);
		POKEINT(OUTPUT_MEMBLOCK,0,0);
	}
	else
	{
		//Failure...
		MessageBoxA(NULL,"Unknown cpChipmunk error 11","Error",MB_OK);
		varFree(mainVar);
		POKEINT(OUTPUT_MEMBLOCK,0,0);
		
	}
	
}

__declspec( dllexport ) void shapeundersegment( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSegmentQueryInfo segmentQuery;
	cpBool hit;
	Variable *var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	cpShape *shape = (cpShape*)var->mPtr;
	hit = cpShapeSegmentQuery(shape,PEEKVECT(INPUT_MEMBLOCK,4),PEEKVECT(INPUT_MEMBLOCK,12),&segmentQuery);
	POKEBYTE(OUTPUT_MEMBLOCK,0,hit);
	POKEFLOAT(OUTPUT_MEMBLOCK,1,segmentQuery.t);
	POKEVECT(OUTPUT_MEMBLOCK,5,segmentQuery.n);
}

#define SIZE_OF_SEGMENT_QUERY_ARRAY 500

int mSegmentQueryIterator;
cpSegmentQueryInfo mSegmentQueries[SIZE_OF_SEGMENT_QUERY_ARRAY];

void resetSegmentQueryData()
{
	memset(mSegmentQueries,0,400*sizeof(cpSegmentQueryInfo));
	mSegmentQueryIterator = 0;
}


void segmentQueryFunc(cpShape *shape, cpFloat t, cpVect n, void *data)
{
	if (mSegmentQueryIterator == SIZE_OF_SEGMENT_QUERY_ARRAY) return;
	mSegmentQueries[mSegmentQueryIterator].shape = shape;
	mSegmentQueries[mSegmentQueryIterator].t = t;
	mSegmentQueries[mSegmentQueryIterator].n = n;
	mSegmentQueryIterator++;
}

__declspec( dllexport ) void segmentquery( const void * _in, int in_size, void * _out, int out_sz )
{
	int index = 0;
	int memindex = 4;
	resetSegmentQueryData();
	cpSpaceSegmentQuery(&mSpace,PEEKVECT(INPUT_MEMBLOCK,0),PEEKVECT(INPUT_MEMBLOCK,8),PEEKINT(INPUT_MEMBLOCK,16),PEEKINT(INPUT_MEMBLOCK,20),segmentQueryFunc,0);
	POKEINT(OUTPUT_MEMBLOCK,0,mSegmentQueryIterator);
	while (index != mSegmentQueryIterator)
	{
		POKEINT(OUTPUT_MEMBLOCK,memindex,((Variable*)(mSegmentQueries[index].shape->data))->mCBPtr);
		POKEFLOAT(OUTPUT_MEMBLOCK,memindex+4,mSegmentQueries[index].t);
		POKEVECT(OUTPUT_MEMBLOCK,memindex+8,mSegmentQueries[index].n);

		memindex+=16;
		index++;
	}
}
__declspec( dllexport ) void segmentqueryfirst( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSegmentQueryInfo info;
	cpShape * shape = cpSpaceSegmentQueryFirst(&mSpace,PEEKVECT(INPUT_MEMBLOCK,0),PEEKVECT(INPUT_MEMBLOCK,8),PEEKINT(INPUT_MEMBLOCK,16),PEEKINT(INPUT_MEMBLOCK,20),&info);
	if (shape == NULL)
	{
		POKEINT(OUTPUT_MEMBLOCK,0,0);
		return;
	}

	POKEINT(OUTPUT_MEMBLOCK,0,((Variable*)shape->data)->mCBPtr);
	POKEFLOAT(OUTPUT_MEMBLOCK,4,info.t);
	POKEVECT(OUTPUT_MEMBLOCK,8,info.n);
}


__declspec( dllexport ) void explosion( const void * _in, int in_size, void * _out, int out_sz )
{
	int i;
	Variable *var;
	cpBody *body;
	cpVect bp;
	float angle;
	float dist;
	float divi;
	float finalPower;
	float power = PEEKFLOAT(INPUT_MEMBLOCK,0);
	cpVect position = PEEKVECT(INPUT_MEMBLOCK,4);
	for (i = 0;i != mVariableHandler.mSize;i++)
	{
		var = mVariableHandler.mPtrArray[i];
		if (var == NULL)continue; 
		if (var->mType != VarTypeBody) continue;
		body = (cpBody*)var->mPtr;
		if (cpBodyIsStatic(body)) continue;
		bp = cpvsub(position,cpBodyGetPos(body));
		dist = cpvlength(bp)+0.1f;
		divi = (power/dist);
		finalPower = -min(power,divi*divi);
		angle = cpvtoangle(bp);
		cpBodyApplyImpulse(body,cpv(cosf(angle)*finalPower,sinf(angle)*finalPower),cpBodyWorld2Local(body,position));
	}
}


__declspec( dllexport ) void shapeunderpoint( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpShape *first = cpSpacePointQueryFirst(&mSpace,PEEKVECT(INPUT_MEMBLOCK,0),(cpLayers)PEEKUINT(INPUT_MEMBLOCK,8),(cpGroup)PEEKUINT(INPUT_MEMBLOCK,12));
	if (first == NULL)
	{
		POKEINT(OUTPUT_MEMBLOCK,0,0);
		return;
	}
	var = (Variable*)first->data;
	POKEINT(OUTPUT_MEMBLOCK,0,var->mCBPtr);
}

__declspec( dllexport ) void usespatialhash( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSpaceUseSpatialHash(&mSpace, PEEKFLOAT(INPUT_MEMBLOCK,0),PEEKINT(INPUT_MEMBLOCK,4));
}


__declspec( dllexport ) void pullall( const void * _in, int in_size, void * _out, int out_sz )
{
	
	int i;
	cpBody *body;
	Variable *var;
	//int bodyCount = 0;
	int index = 0;

	for (i = 0;i != mVariableHandler.mSize;i++)
	{
		var = mVariableHandler.mPtrArray[i];
		if (var == NULL)continue;
		
		//Varmistetaan, ett‰ haetaan vain runkojen tiedot
		if (var->mType != VarTypeBody)continue;
		body = (cpBody*)var->mPtr;

		//Eik‰ staattisen tietoja
		if (cpBodyIsStatic(body))continue;

		//Alkuun cb:n muistipalan osoite
		POKEINT(OUTPUT_MEMBLOCK,index,var->mCBPtr);
		POKEFLOAT(OUTPUT_MEMBLOCK,index+4,radToDeg(cpBodyGetAngle(body)));
		POKEVECT(OUTPUT_MEMBLOCK,index+8,cpBodyGetPos(body));
		POKEFLOAT(OUTPUT_MEMBLOCK,index+16,radToDeg(cpBodyGetAngVel(body)));
		POKEVECT(OUTPUT_MEMBLOCK,index+20,cpBodyGetVel(body));
		index += 28;
		//bodyCount++;
	}
	//POKEINT(OUTPUT_MEMBLOCK,0,bodyCount);

}

__declspec( dllexport ) void setenablecontactgraph( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSpaceSetEnableContactGraph(&mSpace,PEEKINT(INPUT_MEMBLOCK,0));
}


//T‰ll‰ resetoidaan runkoihin kohdistuvat voimat
void resetBodyForces(cpBody *body, void *data)
{

	//Vaatii pieni‰ tarkistuksia, koska muuten nukkuminen ei toimisi
	if ((body->f.x != 0) || (body->f.y != 0) || (body->t != 0)) cpBodyResetForces(body);
}


DWORD WINAPI updateThread( LPVOID lpParam ) ;

__declspec( dllexport ) void update( const void * _in, int in_size, void * _out, int out_sz )
{
	//Lukee ajan joka p‰ivitett‰‰n
	mTimeStep = PEEKFLOAT(INPUT_MEMBLOCK,0);

	//Varmistetaan, ett‰ updatea ei kutsuta kahta kertaa per‰kk‰in...
	if (mThread == NULL)
	{
		//Luodaan s‰ie ja aletaan suorittaa sit‰
		mThread = CreateThread(NULL,0,updateThread,0,0,&mThreadID);
	}
}


DWORD WINAPI updateThread( LPVOID lpParam ) 
{

	clock_t spendTime;
	clock_t startTick;

	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER start_ticks, end_ticks, cputime;

	//Tarkistetaan onko tarkempi kello k‰ytett‰viss‰
	if (!QueryPerformanceFrequency(&ticksPerSecond))
	{
		startTick = clock();
		cpSpaceStep(&mSpace,mTimeStep);
		spendTime = clock() - startTick;
		mUpdateTime = (float)spendTime/CLOCKS_PER_SEC*1000.0f;
	}
	else
	{
		QueryPerformanceCounter(&start_ticks);
		cpSpaceStep(&mSpace,mTimeStep);
		QueryPerformanceCounter(&end_ticks);
		cputime.QuadPart = end_ticks.QuadPart- start_ticks.QuadPart;
		mUpdateTime = (float)cputime.QuadPart/(float)ticksPerSecond.QuadPart*1000.0f;
	}

	//Nollataan voimat
	if (mAutoClearForces) cpSpaceEachBody(&mSpace,resetBodyForces,0);
	
	return 0;
}











inline char *combineStr(const char* str1,const char *str2)
{
	char *ret;		
	int count1 = 0;                /* Length of str1 */
	int count2 = 0;                /* Length of str2 */

	/* find the length of str1 */
	while (str1[count1] != '\0')   /* Increment count till we reach the terminating character*/
	count1++;                   
  
	/* Find the length of str2 */
	while (str2[count2] != '\0')  /* Count characters in second string      */
	count2++;
  
	/* Check that we have enough space for both strings  */
	if(sizeof str1 < count1 + count2 + 1)
	printf("\nYou can't put a quart into a pint pot.");
	else
	{  /* Copy 2nd string to end of the first  */
		ret = (char*)malloc(count1 + count2 + 1);
		count2 = 0;                 /* Reset index for str2 to 0   */
		while(str2[count2] != '\0')  /* Copy up to null from str2   */
			ret[count1++] = str2[count2++];

		ret[count1] = '\0';         /* Make sure we add terminator */
		return ret;
	}
	return NULL;
}

BOOL debugMessageBox(cpBody *body,const char* head)
{
	char str[100];
	cpVect pos;
	pos = cpBodyGetPos(body);
	sprintf(str,"Pos:(%f,%f) Angle: %f \0",pos.x,pos.y,cpBodyGetAngle(body));
	if (MessageBoxA(NULL,str,head,MB_OKCANCEL) == IDOK) return TRUE;
	return FALSE;
}












