/*
	DecoderOregon.h-Biblioteca de decodagem de temperatura
	Oregon Scientific

	// Oregon V2 decoder modfied - Olivier Lebrun
	// Oregon V2 decoder added - Dominique Pierre
	// New code to decode OOK signals from weather sensors, etc.
	// 2010-04-11 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
	// $Id: ookDecoder.pde 5331 2010-04-17 10:45:17Z jcw $
	// Breno Zaidan Martinelli

*/


#ifndef _OREGONDECODER_H
#define _OREGONDECODER_H


#include "Arduino.h"

class DecodeOOK {
protected:
    byte total_bits, bits, flip, state, pos, data[25];
 
    virtual char decode (word width) =0;
 
public:
 
    enum { UNKNOWN, T0, T1, T2, T3, OK, DONE };
 

 	//Métodos
    DecodeOOK ();// Método costrutor
 
    bool nextPulse (word width);
 
    bool isDone ();
 
    const byte* getData (byte& count);
 
    void resetDecoder ();
 
    // add one bit to the packet data buffer
 
    virtual void gotBit (char value);
 
    // store a bit using Manchester encoding
    void manchester (char value);
 
    // move bits to the front so that all the bits are aligned to the end
    void alignTail (byte max);
 
    void reverseBits ();
 
    void reverseNibbles ();
 
    void done ();
};

#endif
 
