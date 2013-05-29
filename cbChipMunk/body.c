#include "cbchipmunk.h"

__declspec( dllexport ) void bodynew( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body;
	Variable * var;
	//Luodaan uusi runko
	body = cpBodyNew(PEEKFLOAT(INPUT_MEMBLOCK,0),PEEKFLOAT(INPUT_MEMBLOCK,4));
	if (body == NULL)
	{
		MessageBoxA(NULL,"cpBodyNew failed.","cbChipmunk error",MB_OK);
		exit(0);
	}
	//Lisätään se variablehandleriin
	var = vhAddBody(&mVariableHandler,body);

	//Laitetaan muuttujalle oikea osoitin CB:n muistipalaan.
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,8);


	cpBodySetPos(body,PEEKVECT(INPUT_MEMBLOCK,12));
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
	POKEFLOAT(OUTPUT_MEMBLOCK,4,radToDeg(cpBodyGetAngle(body)));
	POKEVECT(OUTPUT_MEMBLOCK,8,cpBodyGetPos(body));
	POKEFLOAT(OUTPUT_MEMBLOCK,16,radToDeg(cpBodyGetAngVel(body)));
	POKEVECT(OUTPUT_MEMBLOCK,20,cpBodyGetVel(body));
	POKEFLOAT(OUTPUT_MEMBLOCK,28,cpBodyGetMass(body));
	POKEFLOAT(OUTPUT_MEMBLOCK,32,cpBodyGetMoment(body));
	
	//Lisätään runko fysiikkamaailmaan, että se päivitettäisiin.
	cpSpaceAddBody(&mSpace,body);
}

__declspec( dllexport ) void bodynewstatic( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body = cpBodyNew(INFINITE,INFINITE);
	Variable * var = vhAddBody(&mVariableHandler,body);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);

	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
	POKEFLOAT(OUTPUT_MEMBLOCK,4,radToDeg(cpBodyGetAngle(body)));
	POKEVECT(OUTPUT_MEMBLOCK,8,cpBodyGetPos(body));
	POKEFLOAT(OUTPUT_MEMBLOCK,16,radToDeg(cpBodyGetAngVel(body)));
	POKEVECT(OUTPUT_MEMBLOCK,20,cpBodyGetVel(body));
	
	cpSpaceAddBody(&mSpace,body);
}

static double gravitationPower = 2.0;
static double gravitationForce = 1.0;
static cpBody *gravitationTarget = 0;

void applyGravitationToBody(cpBody *b, void *d) {
	if (b != gravitationTarget && !cpBodyIsStatic(b)) {
		double force;
		if (gravitationPower == 2.0) {
			force = gravitationForce * b->m / cpvdistsq(b->p, gravitationTarget->p);
		} else {
			force = gravitationForce * b->m / pow(cpvdist(b->p, gravitationTarget->p), gravitationPower);
		}
		cpBodyApplyForce(b, cpvmult(cpvnormalize(cpvsub(gravitationTarget->p, b->p)), force), cpvzero);
	}
}

__declspec ( dllexport ) void applygravitation( const void * _in, int in_size, void * _out, int out_sz ) {
	Variable *var;
	var = vhGetVariable(&mVariableHandler, PEEKINT(INPUT_MEMBLOCK, 0));
	gravitationPower = PEEKFLOAT(INPUT_MEMBLOCK, 8);
	gravitationForce = PEEKFLOAT(INPUT_MEMBLOCK, 4);
	if (var->mType != VarTypeBody) 
	{
		MessageBoxA(NULL,"The first parameter of cpApplyGravitation must be cpBody","Chipmunk error",MB_OK);
		return;
	}
	gravitationTarget = (cpBody*)var->mPtr;

	cpSpaceEachBody(&mSpace, &applyGravitationToBody, 0);

}


__declspec( dllexport ) void isbodysleeping( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	if (var->mType != VarTypeBody)
	{
		MessageBoxA(NULL,"cpIsBodySleeping's parameter must be cpBody","Chipmunk error",MB_OK);
		POKEINT(OUTPUT_MEMBLOCK,0,0);
		return;
	}
	POKEINT(OUTPUT_MEMBLOCK,0,cpBodyIsSleeping(((cpBody*)var->mPtr)));
}

__declspec( dllexport ) void bodyapplyforce( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body;
	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	body = (cpBody*)var->mPtr;
	cpBodyApplyForce(body,PEEKVECT(INPUT_MEMBLOCK,4),PEEKVECT(INPUT_MEMBLOCK,12));
}

__declspec( dllexport ) void bodyapplyimpulse( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body;
	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	body = (cpBody*)var->mPtr;
	cpBodyApplyImpulse(body,PEEKVECT(INPUT_MEMBLOCK,4),PEEKVECT(INPUT_MEMBLOCK,12));
}



__declspec( dllexport ) void bodyworldtolocal( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body = (cpBody*)vhGetVariable(&mVariableHandler,(INPUT_MEMBLOCK,0))->mPtr;
	POKEVECT(OUTPUT_MEMBLOCK,0,cpBodyWorld2Local(body,PEEKVECT(INPUT_MEMBLOCK,4)));
}



__declspec( dllexport ) void bodylocaltoworld( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body = (cpBody*)vhGetVariable(&mVariableHandler,(INPUT_MEMBLOCK,0))->mPtr;
	POKEVECT(OUTPUT_MEMBLOCK,0,cpBodyLocal2World(body,PEEKVECT(INPUT_MEMBLOCK,4)));
}


#define ARBITER_LIST_SIZE 15
int mArbiterIndex = 0;
cpArbiter *mArbiterList[ARBITER_LIST_SIZE];

void listArbiter(cpBody *body, cpArbiter *arbiter, void *data)
{
	if (mArbiterIndex == ARBITER_LIST_SIZE) return;
	mArbiterList[mArbiterIndex] = arbiter;
	mArbiterIndex++;
}


__declspec( dllexport ) void getarbiters( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body;
	cpBody *body1;
	cpBody *body2;
	cpShape *shape1;
	cpShape *shape2;
	int index;
	cpArbiter *arbiter;
	cpContactPointSet pointset;
	int i;
	int i2;
	cpVect tImpulse;
	

	//Haetaan runko jonka törmäyksiä tutkitaan.
	body = (cpBody*)(vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0))->mPtr);

	//Alustetaan listan osoitin.
	mArbiterIndex = 0;

	//Kutsutaan listArbiter funktiota kaikkille törmäyksille

	cpBodyEachArbiter(body,listArbiter,0);

	//Laitetaan muistipalan alkuun paljonko törmäyksiä oli.
	POKEINT(OUTPUT_MEMBLOCK,0,mArbiterIndex);
	
	//Koska määrä on jo kirjoitettu niin muistipalan index alkaa 4:stä.
	index = 4;

	//Käydään läpi kaikki törmäykset
	for (i = 0;i < mArbiterIndex;i++)
	{
		arbiter = mArbiterList[i];
		cpArbiterGetBodies(arbiter,&body1,&body2);
		cpArbiterGetShapes(arbiter,&shape1,&shape2);

		//Haetaan törmäyspisteet
		pointset = cpArbiterGetContactPointSet(arbiter);
		//Kirjoitetaan törmäysten määrä muistipalaan.
		POKEINT(OUTPUT_MEMBLOCK,index,pointset.count);
		if (body == body1)
		{
			POKEINT(OUTPUT_MEMBLOCK,index+4,((Variable*)body2->data)->mCBPtr);
			POKEINT(OUTPUT_MEMBLOCK,index+8,((Variable*)shape1->data)->mCBPtr);
			POKEINT(OUTPUT_MEMBLOCK,index+12,((Variable*)shape2->data)->mCBPtr);
		}
		else //Tämä tuskin koskaan toteutuu, mutta asia pitäisi varmistaa
		{
			POKEINT(OUTPUT_MEMBLOCK,index+4,((Variable*)body1->data)->mCBPtr);
			POKEINT(OUTPUT_MEMBLOCK,index+8,((Variable*)shape2->data)->mCBPtr);
			POKEINT(OUTPUT_MEMBLOCK,index+12,((Variable*)shape1->data)->mCBPtr);
		}
		POKEVECT(OUTPUT_MEMBLOCK,index+16,cpArbiterTotalImpulse(arbiter));

		POKEVECT(OUTPUT_MEMBLOCK,index+24,cpArbiterTotalImpulseWithFriction(arbiter));

		index +=32;
		for (i2 = 0; i2 < pointset.count;i2++)
		{
			POKEVECT(OUTPUT_MEMBLOCK,index,pointset.points[i2].point);
			
			index += 8;
		}
	}
}

__declspec( dllexport ) void applytorque( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body;
	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	body = (cpBody*)var->mPtr;
	cpBodySetTorque(body,cpBodyGetTorque(body)-PEEKFLOAT(INPUT_MEMBLOCK,4));
}

__declspec( dllexport ) void reindexshape( const void * _in, int in_size, void * _out, int out_sz )
{
	cpShape *body = (cpShape*)vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0))->mPtr;
	cpSpaceReindexShape(&mSpace,body);
}

__declspec( dllexport ) void reindexbody( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body = (cpBody*)vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0))->mPtr;
	cpSpaceReindexShapesForBody(&mSpace,body);
}

__declspec( dllexport ) void reindexspace( const void * _in, int in_size, void * _out, int out_sz )
{
	cpSpaceReindexStatic(&mSpace);
}


/*

//Wings... :P

static void wingUpdateVelocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
	cpFloat w_limit;
	body->v = cpvclamp(cpvadd(cpvmult(body->v, damping), cpvmult(cpvadd(gravity, cpvmult(body->f, body->m_inv)), dt)), body->v_limit);
	
	w_limit = body->w_limit;
	body->w = cpfclamp(body->w*damping + body->t*body->i_inv*dt, -w_limit, w_limit);
	
	

	cpBodySanityCheck(body);
}

__declspec( dllexport ) void bodysettowing( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body;
	Variable *liftCoefficientVar;
	Variable *var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	
	body = (cpBody*)var->mPtr;
	body->velocity_func = (cpBodyVelocityFunc)&wingUpdateVelocity;
	
	liftCoefficientVar = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,4));
	if (liftCoefficientVar->mType != VarTypeDataArray)
	{
		MessageBoxA(NULL,"Invalid liftcoefficient data-array","cbChipmunk error",MB_OK);
		exit(0);
	}

	//Osoitin nostekäyrään...
	var->mData = liftCoefficientVar->mPtr;
	
}


*/






/*Kaikkea turhaa


__declspec( dllexport ) void setposition( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	if (var->mType != VarTypeBody)
	{
		MessageBoxA(NULL,"You can only set position of body","Error",MB_OK);
		return;
	}
	body = (cpBody*)var->mPtr;
	cpBodySetPos(body,PEEKVECT(INPUT_MEMBLOCK,4));
}
__declspec( dllexport ) void setangle( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpBody *body;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	if (var->mType != VarTypeBody)
	{
		MessageBoxA(NULL,"You can only set angle of body","Error",MB_OK);
		return;
	}
	body = (cpBody*)var->mPtr;
	cpBodySetAngle(body,degToRad(PEEKFLOAT(INPUT_MEMBLOCK,4)));
}
*/