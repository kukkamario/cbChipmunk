#ifndef __CBCHIPMUNK_H__
#define __CBCHIPMUNK_H__

#include "common.h"

#define INPUT_MEMBLOCK _in
#define OUTPUT_MEMBLOCK _out
#define INPUT_MEMBLOCK_SIZE in_sz
#define OUTPUT_MEMBLOCK_SIZE out_sz
#define POKEBYTE( __mem__, __byte__, __value__ ) ( ( * ( ( unsigned char * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) ) = __value__ )
#define POKESHORT( __mem__, __byte__, __value__ ) ( ( * ( ( unsigned short int * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) ) = __value__ )
#define POKEINT( __mem__, __byte__, __value__ ) ( ( * ( ( int *) ( ( ( char * ) __mem__ ) + __byte__ ) ) ) = __value__ )
#define POKEUINT( __mem__, __byte__, __value__ ) ( ( * ( ( unsigned int *) ( ( ( char * ) __mem__ ) + __byte__ ) ) ) = __value__ )
#define POKEFLOAT( __mem__, __byte__, __value__ ) ( ( * ( ( float * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) ) = __value__ )
#define PEEKBYTE( __mem__, __byte__ ) ( * ( ( unsigned char * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) )
#define PEEKSHORT( __mem__, __byte__ ) ( * ( ( unsigned short int * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) )
#define PEEKINT( __mem__, __byte__ ) ( * ( ( int * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) )
#define PEEKUINT( __mem__, __byte__ ) ( * ( ( unsigned int * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) )
#define PEEKFLOAT( __mem__, __byte__ ) ( * ( ( float * ) ( ( ( char * ) __mem__ ) + __byte__ ) ) )


/*
* Makrojen k‰yttˆ:
*
* CB_FUNCTION( funktionnimi ) k‰ytet‰‰n funktion tavallisen m‰‰rittelytavan (void funktionnimi ja niin edelleen) tilalla
* Esimerkki k‰ytˆst‰:
* CB_FUNCTION( esimerkkifunktio )
* {
*
*    <t‰h‰n funktion koodi>
*
* }
*
* INPUT_MEMBLOCK on sama kuin _in
*
* OUTPUT_MEMBLOCK on sama kuin _out
*
* INPUT_MEMBLOCK_SIZE on sama kuin in_sz
*
* OUTPUT_MEMBLOCK_SIZE on sama kuin out_sz
*
* POKE* ja PEEK* -makrot samoja kuin CB:n vastaavat funktiot
* Tietotyyppimuunnokset
* ------------------------------------------
* CoolBasic | C ja C++
* ------------------------------------------
* Integer    |   int / int32_t
* Short    |   unsigned short int / uint16_t
* Byte       |   unsigned char / uint8_t
* Float    |   float
* String    |   unsigned char * (Ks. HUOM)
* ------------------------------------------
* HUOM! Merkkijonojen k‰sittely on C:ss‰
* erilainen kuin CoolBasicissa. C:n merkki-
* jonot ovat osoittimia merkkijonon alkuun,
* eiv‰tk‰ ole "varsinaisia" tietotyyppej‰.
* ------------------------------------------
* POKEBYTE( muistipala, tavuja muistipalan alusta, arvo )
* PEEKBYTE( muisti, tavuja )
* POKESHORT( muisti, tavuja, arvo )
* PEEKSHORT( muisti, tavuja )
* POKEINT( muisti, tavuja, arvo )
* PEEKINT( muisti, tavuja )
* POKEFLOAT( muisti, tavuja, arvo )
* PEEKFLOAT( muisti, tavuja )
*
* Toiminta perustuu siihen, ett‰ ensin osoitin muutetaan
* osoittimeksi char:iin (tavun tarkkuus), siirryt‰‰n
* haluttu m‰‰r‰ tavuja eteenp‰in, muunnetaan osoittimen
* tietotyyppi halutuksi ja kirjoitetaan data osoittimen
* osoittamaan muistiosoitteeseen.
* Prosessi on muuten sama PEEK* -makroilla, mutta
* kirjoitus j‰‰ pois
*/
#include "variablehandler.h"

typedef enum State_
{
	Unloaded = 0,
	Loaded
} State;

//Muuttuja luettelo...
extern VariableHandler mVariableHandler;

//Chipmunkin "maailma"
extern cpSpace mSpace;

extern cpVect mWind;

extern State mState;

//Luetaan vektori muistipalasta kohdasta p.
cpVect PEEKVECT(const void *mem,int p);

//Kirjoitettaan vektori muistipalaan kohtaan p.
void POKEVECT(void *mem,int p,const cpVect vec);

//Asteet radiaaneiksi
float degToRad(float deg);

//Radiaanit asteiksi
float radToDeg(float rad);

#endif
