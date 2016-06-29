/*
	AffichageOregon.h-Biblioteca de visualisacao de dados
	Oregon Scientific

	// Oregon V2 decoder modfied - Olivier Lebrun
	// Oregon V2 decoder added - Dominique Pierre
	// New code to decode OOK signals from weather sensors, etc.
	// 2010-04-11 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
	// $Id: ookDecoder.pde 5331 2010-04-17 10:45:17Z jcw $
	// Breno Zaidan Martinelli

*/

#ifndef _AFFICHAGEOREGON_H
#define _AFFICHAGEOREGON_H

#include "Arduino.h"
#include "OregonDecoder.h"
#include "OregonDecoderV2.h"

struct permission
{
    boolean teste;
    float TEMPERATURA;
    byte ID;

};

typedef struct permission Permission;

class AffichageOregon{
public:

	AffichageOregon();

	//void ext_int_1(volatile word pulse);
	 
	float temperature(const byte* data,float TEMP);
	 
	byte humidity(const byte* data);
	 
	// Ne retourne qu'un apercu de l'etat de la baterie : 10 = faible
	byte battery(const byte* data);
	 
	byte channel(const byte* data);

	Permission reportSerial (const char* s, class DecodeOOK& decoder,float TEMP);
};

#endif