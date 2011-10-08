#include "cbchipmunk.h"

__declspec( dllexport ) void pinjointnew( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body1;
	cpBody *body2;
	cpConstraint *constraint;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,4));
	body1 = (cpBody*)var->mPtr;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,8));
	body2 = (cpBody*)var->mPtr;

	constraint = cpPinJointNew(body1,body2,PEEKVECT(INPUT_MEMBLOCK,12),PEEKVECT(INPUT_MEMBLOCK,20));
	cpSpaceAddConstraint(&mSpace,constraint);
	var = vhAddConstraint(&mVariableHandler,constraint);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}

__declspec( dllexport ) void pivotjointnew( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body1;
	cpBody *body2;
	cpConstraint *constraint;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,4));
	body1 = (cpBody*)var->mPtr;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,8));
	body2 = (cpBody*)var->mPtr;

	constraint = cpPivotJointNew2(body1,body2,PEEKVECT(INPUT_MEMBLOCK,12),PEEKVECT(INPUT_MEMBLOCK,20));
	cpSpaceAddConstraint(&mSpace,constraint);
	var = vhAddConstraint(&mVariableHandler,constraint);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}


__declspec( dllexport ) void slidejointnew( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body1;
	cpBody *body2;
	cpConstraint *constraint;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,4));
	body1 = (cpBody*)var->mPtr;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,8));
	body2 = (cpBody*)var->mPtr;

	constraint = cpSlideJointNew(body1,body2,PEEKVECT(INPUT_MEMBLOCK,12),PEEKVECT(INPUT_MEMBLOCK,20),PEEKFLOAT(INPUT_MEMBLOCK,28),PEEKFLOAT(INPUT_MEMBLOCK,32));
	cpSpaceAddConstraint(&mSpace,constraint);
	var = vhAddConstraint(&mVariableHandler,constraint);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}

__declspec( dllexport ) void rotarylimitjointnew( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body1;
	cpBody *body2;
	cpConstraint *constraint;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,4));
	body1 = (cpBody*)var->mPtr;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,8));
	body2 = (cpBody*)var->mPtr;

	constraint = cpRotaryLimitJointNew(body1,body2,degToRad(PEEKFLOAT(INPUT_MEMBLOCK,12)),degToRad(PEEKFLOAT(INPUT_MEMBLOCK,16)));
	cpSpaceAddConstraint(&mSpace,constraint);
	var = vhAddConstraint(&mVariableHandler,constraint);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}


__declspec( dllexport ) void groovejointnew( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body1;
	cpBody *body2;
	cpConstraint *constraint;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,4));
	body1 = (cpBody*)var->mPtr;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,8));
	body2 = (cpBody*)var->mPtr;

	constraint = cpGrooveJointNew(body1,body2,PEEKVECT(INPUT_MEMBLOCK,12),PEEKVECT(INPUT_MEMBLOCK,20),PEEKVECT(INPUT_MEMBLOCK,28));
	cpSpaceAddConstraint(&mSpace,constraint);
	var = vhAddConstraint(&mVariableHandler,constraint);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}

__declspec( dllexport ) void dampedspringnew( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body1;
	cpBody *body2;
	cpConstraint *constraint;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,4));
	body1 = (cpBody*)var->mPtr;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,8));
	body2 = (cpBody*)var->mPtr;

	constraint = cpDampedSpringNew(body1,body2,PEEKVECT(INPUT_MEMBLOCK,12),PEEKVECT(INPUT_MEMBLOCK,20),PEEKFLOAT(INPUT_MEMBLOCK,28),PEEKFLOAT(INPUT_MEMBLOCK,32),PEEKFLOAT(INPUT_MEMBLOCK,36));
	cpSpaceAddConstraint(&mSpace,constraint);
	var = vhAddConstraint(&mVariableHandler,constraint);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}