#include "cbchipmunk.h"

__declspec( dllexport ) void shapenewstatic( const void * _in, int in_size, void * _out, int out_sz )
{
	Variable *var;
	cpShape *shape;
	cpVect offset;
	cpVect translated1;
	cpVect translated2;
	cpVect* vertices;
	cpBB box;
	int size;
	int i;
	float w;
	float h;
	float friction;
	float restitution;
	cpGroup group;
	cpLayers layers;
	unsigned char geometryType;

	offset = PEEKVECT(INPUT_MEMBLOCK,4);
	friction = PEEKFLOAT(INPUT_MEMBLOCK,12);
	restitution = PEEKFLOAT(INPUT_MEMBLOCK,16);
	layers = PEEKUINT(INPUT_MEMBLOCK,20);
	group = PEEKUINT(INPUT_MEMBLOCK,24);
	
	geometryType = PEEKBYTE(INPUT_MEMBLOCK,28);
	switch (geometryType)
	{
	case 4://Box
		w = PEEKFLOAT(INPUT_MEMBLOCK,29);
		h = PEEKFLOAT(INPUT_MEMBLOCK,33);
		box.l = offset.x-w*0.5;
		box.r = box.l + w;
		box.t = offset.y + h*0.5;
		box.b = box.t - h;
		shape = cpBoxShapeNew2(mSpace.staticBody,box);
		break;
	case 5://Circle
		shape = cpCircleShapeNew(mSpace.staticBody,PEEKFLOAT(INPUT_MEMBLOCK,29),offset);
		break;
	case 6://Polygon...
		size = PEEKINT(INPUT_MEMBLOCK,29);
		vertices = (cpVect*)malloc(size*sizeof(cpVect));
		for (i = 0;i != size;i++)
		{
			vertices[i] = PEEKVECT(INPUT_MEMBLOCK,33+i*8);
		}
		shape = cpPolyShapeNew(mSpace.staticBody,size,vertices,offset);
		free(vertices);
		break;
	case 7://Segment
		translated1 = PEEKVECT(INPUT_MEMBLOCK,29);
		translated2 = PEEKVECT(INPUT_MEMBLOCK,37);

		translated1.x += offset.x;
		translated1.y += offset.y;
		
		translated2.x += offset.x;
		translated2.y += offset.y;

		shape = cpSegmentShapeNew(mSpace.staticBody,translated1,translated2,PEEKFLOAT(INPUT_MEMBLOCK,45));
		break;
	}
	shape->e = restitution;
	shape->u = friction;
	shape->group = group;
	shape->layers = layers;
	cpSpaceAddShape(&mSpace,shape);
	

	var = vhAddShape(&mVariableHandler,shape);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,0);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}



__declspec( dllexport ) void shapenew( const void * _in, int in_size, void * _out, int out_sz )
{
	cpBody *body;
	Variable *var;
	cpShape *shape;
	cpVect offset;
	cpVect translated1;
	cpVect translated2;
	cpVect *vertices;
	int size;
	int i;
	cpBB box;
	float w;
	float h;
	float friction;
	float restitution;
	cpGroup group;
	cpLayers layers;
	unsigned char geometryType;

	var = vhGetVariable(&mVariableHandler,PEEKINT(INPUT_MEMBLOCK,0));
	body = (cpBody*)var->mPtr;
	offset = PEEKVECT(INPUT_MEMBLOCK,8);
	friction = PEEKFLOAT(INPUT_MEMBLOCK,16);
	restitution = PEEKFLOAT(INPUT_MEMBLOCK,20);
	group = PEEKUINT(INPUT_MEMBLOCK,28);
	layers = PEEKUINT(INPUT_MEMBLOCK,24);

	geometryType = PEEKBYTE(INPUT_MEMBLOCK,32);
	switch (geometryType)
	{
	case 4://Box
		w = PEEKFLOAT(INPUT_MEMBLOCK,33);
		h = PEEKFLOAT(INPUT_MEMBLOCK,37);
		box.l = offset.x-w*0.5;
		box.r = box.l + w;
		box.t = offset.y + h*0.5;
		box.b = box.t - h;
		shape = cpBoxShapeNew2(body,box);
		break;
	case 5://Circle
		shape = cpCircleShapeNew(body,PEEKFLOAT(INPUT_MEMBLOCK,33),offset);
		break;
	case 6://Polygon...
		size = PEEKINT(INPUT_MEMBLOCK,33);
		vertices = (cpVect*)malloc(size*sizeof(cpVect));
		for (i = 0;i != size;i++)
		{
			vertices[i] = PEEKVECT(INPUT_MEMBLOCK,37+i*8);
		}
		if (!cpPolyValidate(vertices,size)) {MessageBox(NULL,"Invalid polygon.","Error",MB_OK);exit(0);};
		shape = cpPolyShapeNew(body
			,size,vertices,offset);
		free(vertices);
		break;
	case 7://Segment
		translated1 = PEEKVECT(INPUT_MEMBLOCK,33);
		translated2 = PEEKVECT(INPUT_MEMBLOCK,41);

		translated1.x += offset.x;
		translated1.y += offset.y;
		
		translated2.x += offset.x;
		translated2.y += offset.y;

		shape = cpSegmentShapeNew(body,translated1,translated2,PEEKFLOAT(INPUT_MEMBLOCK,49));
		break;
	}
	shape->e = restitution;
	shape->u = friction;
	shape->group = group;
	shape->layers = layers;
	cpSpaceAddShape(&mSpace,shape);
	

	var = vhAddShape(&mVariableHandler,shape);
	var->mCBPtr = PEEKINT(INPUT_MEMBLOCK,4);
	POKEINT(OUTPUT_MEMBLOCK,0,var->mLocalHandle);
}