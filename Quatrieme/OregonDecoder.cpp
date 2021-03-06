/*
	DecoderOregon.cpp-Biblioteca de decodagem de temperatura
	Oregon Scientific

	// Oregon V2 decoder modfied - Olivier Lebrun
	// Oregon V2 decoder added - Dominique Pierre
	// New code to decode OOK signals from weather sensors, etc.
	// 2010-04-11 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
	// $Id: ookDecoder.pde 5331 2010-04-17 10:45:17Z jcw $
	// Breno Zaidan Martinelli

*/

#include "Arduino.h"
#include "OregonDecoder.h"

DecodeOOK::DecodeOOK () { resetDecoder(); }

bool DecodeOOK::nextPulse (word width) {
    if (state != DONE)
        switch (decode(width)) {
            case -1: resetDecoder(); break;
            case 1:  done(); break;
        }
    return isDone();
    }

bool DecodeOOK::isDone () { return state == DONE; }

const byte* DecodeOOK::getData (byte& count) {
    count = pos;
    return data; 
}

void DecodeOOK::resetDecoder () {
    total_bits = bits = pos = flip = 0;
    state = UNKNOWN;
}

    
void DecodeOOK::gotBit (char value) {
    total_bits++;
    byte *ptr = data + pos;
    *ptr = (*ptr >> 1) | (value << 7);
    if (++bits >= 8) {
        bits = 0;
        if (++pos >= sizeof data) {
            resetDecoder();
            return;
        }
    }
    state = OK;
}

void DecodeOOK::manchester (char value) {
    flip ^= value; // manchester code, long pulse flips the bit
    gotBit(flip);
}

void DecodeOOK::alignTail (byte max) {
    // align bits
    if (bits != 0) {
        data[pos] >>= 8 - bits;
        for (byte i = 0; i < pos; ++i)
            data[i] = (data[i] >> bits) | (data[i+1] << (8 - bits));
            bits = 0;
        }
        // optionally shift bytes down if there are too many of 'em
    if (max > 0 && pos > max) {
        byte n = pos - max;
        pos = max;
        for (byte i = 0; i < pos; ++i)
            data[i] = data[i+n];
        }
    }

void DecodeOOK::reverseBits () {
    for (byte i = 0; i < pos; ++i) {
        byte b = data[i];
        for (byte j = 0; j < 8; ++j) {
            data[i] = (data[i] << 1) | (b & 1);
            b >>= 1;
        }
    }
}

void DecodeOOK::reverseNibbles () {
    for (byte i = 0; i < pos; ++i)
        data[i] = (data[i] << 4) | (data[i] >> 4);
}

void DecodeOOK::done () {
    while (bits)
        gotBit(0); // padding
    state = DONE;
}


 
