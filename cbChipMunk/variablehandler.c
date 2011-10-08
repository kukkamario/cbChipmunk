#include "variablehandler.h"

Variable *varAlloc(void)
{
	return (Variable*)malloc(sizeof(Variable));
}

Variable *vhGetVariable(const VariableHandler* handler,int index)
{
	return handler->mPtrArray[index];
}

Variable *vhRemoveVariable(VariableHandler *handler,int index)
{
	Variable *var = handler->mPtrArray[index];
	handler->mPtrArray[index] = NULL;
	return var;
}

void varFree(Variable *var)
{
	free(var);
}

void vhDestroy(VariableHandler *handler)
{
	int i;
	Variable *var;

	for (i = 0;i != handler->mSize;i++)
	{
		var = handler->mPtrArray[i];
		if (var != 0)
		{
			varFree(var);
		}
	}
	free(handler->mPtrArray);
	handler->mPtrArray = 0;
	handler->mSize = 0;
}

void vhInit(VariableHandler *handler)
{
	handler->mSize = 5;
	handler->mPtrArray = (Variable**)malloc(handler->mSize*sizeof(Variable*));
	memset(handler->mPtrArray,0,handler->mSize*sizeof(Variable*));
}



void vhFree(VariableHandler *handler)
{
	vhDestroy(handler);
	free(handler);

}

Variable *vhAddPtr(VariableHandler *handler,void *ptr)
{
	Variable *var;
	int i;
	unsigned int newSize;
	void *newArray;

	for (i = 0;i != handler->mSize;i++)
	{
		if (handler->mPtrArray[i] == NULL)
		{
			var = varAlloc();
			var->mCBPtr = 0;
			var->mData = 0;
			var->mLocalHandle = i;
			var->mPtr = ptr;
			var->mType = VarTypePtr;
			handler->mPtrArray[i] = var;
			return var;
		}
	}

	//resize
	newSize = handler->mSize * 2;
	newArray = malloc(newSize*sizeof(void*));
	memcpy(newArray,handler->mPtrArray,handler->mSize*sizeof(void*));
	handler->mPtrArray = (Variable**)newArray;
	

	var = varAlloc();
	var->mCBPtr = 0;
	var->mLocalHandle = handler->mSize;
	var->mPtr = ptr;
	var->mData = 0;
	var->mType = VarTypePtr;
	handler->mPtrArray[handler->mSize] = var;

	for (i = handler->mSize+1;i != newSize;i++)
	{
		handler->mPtrArray[i] = NULL;
	}
	handler->mSize = newSize;

	return var;
}


//Lisää rungon muuttujalistaan
Variable *vhAddBody(VariableHandler *handler,cpBody *body)
{
	Variable *var;
	var = vhAddPtr(handler,body);
	var->mType = VarTypeBody;
	body->data = var;
	return var;
}

//Lisää törmäysmuodon muuttujalistaan
Variable *vhAddShape(VariableHandler *handler,cpShape *shape)
{
	Variable *var;
	var = vhAddPtr(handler,shape);
	var->mType = VarTypeShape;
	shape->data = var;
	return var;
}


//Lisää muuttujalistaan liitoksen
Variable *vhAddConstraint(VariableHandler *handler,cpConstraint *constraint)
{
	Variable *var;
	var = vhAddPtr(handler,constraint);
	var->mType = VarTypeConstraint;
	constraint->data = var;
	return var;
}

Variable *vhAddDataArray(VariableHandler *handler,DataArray *a)
{
	Variable *var = vhAddPtr(handler,a);
	var->mType = VarTypeDataArray;
	a->mVariable = var;
	return var;
}