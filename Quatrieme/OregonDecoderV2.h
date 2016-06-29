/*
	OregonDecoderV2.h-Biblioteca de decodagem de temperatura
	Oregon Scientific

	// Oregon V2 decoder modfied - Olivier Lebrun
	// Oregon V2 decoder added - Dominique Pierre
	// New code to decode OOK signals from weather sensors, etc.
	// 2010-04-11 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
	// $Id: ookDecoder.pde 5331 2010-04-17 10:45:17Z jcw $
	// Breno Zaidan Martinelli

*/

#ifndef _OREGONDECODERV2_H
#define _OREGONDECODERV2_H

#include "Arduino.h"
#include "OregonDecoder.h"

class OregonDecoderV2 : public DecodeOOK {
  public:   
 
    OregonDecoderV2();
 
    // add one bit to the packet data buffer
    virtual void gotBit (char value);
 
    virtual char decode (word width);
};

#endif