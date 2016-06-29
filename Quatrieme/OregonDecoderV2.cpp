/*
	OregonDecoderV2.cpp-Biblioteca de decodagem de temperatura
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
#include "OregonDecoderV2.h"


OregonDecoderV2::OregonDecoderV2() {}

void OregonDecoderV2::gotBit (char value) {
    if(!(total_bits & 0x01))
    {
        data[pos] = (data[pos] >> 1) | (value ? 0x80 : 00);
    }
    total_bits++;
    pos = total_bits >> 4;
    if (pos >= sizeof data) {
        Serial.println("sizeof data");
        resetDecoder();
        return;
    }
    state = OK;
}

char OregonDecoderV2::decode (word width) {
       if (200 <= width && width < 1200) {
            //Serial.println(width);
            byte w = width >= 700;
 
            switch (state) {
                case UNKNOWN:
                    if (w != 0) {
                        // Long pulse
                        ++flip;
                    } else if (w == 0 && 24 <= flip) {
                        // Short pulse, start bit
                        flip = 0;
                        state = T0;
                    } else {
                        // Reset decoder
                        return -1;
                    }
                    break;
                case OK:
                    if (w == 0) {
                        // Short pulse
                        state = T0;
                    } else {
                        // Long pulse
                        manchester(1);
                    }
                    break;
                case T0:
                    if (w == 0) {
                      // Second short pulse
                        manchester(0);
                    } else {
                        // Reset decoder
                        return -1;
                    }
                    break;
              }
        } else if (width >= 2500  && pos >= 8) {
            return 1;
        } else {
            return -1;
        }
        return 0;
    }