#ifndef VARIABLE_HANDLER_H
#define VARIABLE_HANDLER_H

#include "common.h"
#include "dataarray.h"

typedef enum VariableType_
{
	VarTypePtr = 0,
	VarTypeBody,
	VarTypeShape,
	VarTypeConstraint,
	VarTypeDataArray
} VariableType;



//Variable on muuttuja joka
typedef struct Variable_
{
	//CB:n muistipalan osoitin
	int mCBPtr;

	//Osoittaa paikan VariableHandlerin listassa
	int mLocalHandle;

	//Muuttujan tyyppi
	VariableType mType;

	//Osoitin muuttujan dataan.
	void *mPtr;

	//T‰m‰ on varattu ylim‰‰r‰ist‰ dataa varten. Yleens‰ 0:lla.
	void *mData;
} Variable;

/** VariableHandler pit‰‰ listaa kaikista muuttujista joita pit‰‰ pysty‰ hallitsemaan CB:n puolelta.
 * Se on k‰yt‰nnˆss‰ vain lista kaikista Variableista.*/
typedef struct VariableHandler_
{
	unsigned int mSize;
	Variable **mPtrArray;
} VariableHandler;
/** Alustaa VariableHandlerin.
*/
void vhInit(VariableHandler *handler);

/**
 * Tuhoaa VariableHandlerin (kutsuu vhDestroyta) ja vapauttaa sen varaaman muistin (free)
 * Katso vhDestroy
*/
void vhFree(VariableHandler *handler);

/**
 * Tuhoaa VariableHandlerin ja poistaa kaikki siin‰ olevat muuttujat (varFree).
*/
void vhDestroy(VariableHandler *handler);

/** Palauttaa muuttujan indeksist‰ index.
*/
Variable *vhGetVariable(const VariableHandler* handler,int index);

/** Vapauttaa muuttujan, mutta ei sen sis‰lt‰m‰‰ dataa
*/
void varFree(Variable *var);

/** Varaa muistia Variablelle
*/
Variable *varAlloc(void);

Variable *vhAddBody(VariableHandler *handler,cpBody *body);

Variable *vhAddConstraint(VariableHandler *handler,cpConstraint *constraint);

Variable *vhAddShape(VariableHandler *handler,cpShape *shape);

Variable *vhAddPtr(VariableHandler *handler,void *ptr);


Variable *vhAddDataArray(VariableHandler *handler,DataArray *a);

Variable *vhRemoveVariable(VariableHandler *handler,int index);


#endif