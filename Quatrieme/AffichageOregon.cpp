/*
	AffichageOregon.cpp-Biblioteca de visualisacao de dados
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
#include "AffichageOregon.h"



AffichageOregon::AffichageOregon(){};

/*void AffichageOregon::ext_int_1(volatile word pulse)
{
    static word last;
    // determine the pulse length in microseconds, for either polarity
    pulse = micros() - last;
    last += pulse;
}*/

float AffichageOregon::temperature(const byte* data,float TEMP)
{
    int sign = (data[6]&0x8) ? -1 : 1;
    float temp = ((data[5]&0xF0) >> 4)*10 + (data[5]&0xF) + (float)(((data[4]&0xF0) >> 4) / 10.0);
    TEMP=sign * temp;
    return sign * temp;
}

byte AffichageOregon::humidity(const byte* data)
{
    return (data[7]&0xF) * 10 + ((data[6]&0xF0) >> 4);
}

byte AffichageOregon::battery(const byte* data)
{
    return (data[4] & 0x4) ? 10 : 90;
}

byte AffichageOregon::channel(const byte* data)
{
    byte channel;
    switch (data[2])
    {
        case 0x10:
            channel = 1;
            break;
        case 0x20:
            channel = 2;
            break;
        case 0x40:
            channel = 3;
            break;
     }
 
     return channel;
}

Permission AffichageOregon::reportSerial (const char* s, class DecodeOOK& decoder,float TEMP)
{
    Permission P1;
    byte pos;
    const byte* data = decoder.getData(pos);
    Serial.print(s);
    Serial.print(' ');
    for (byte i = 0; i < pos; ++i) {
        Serial.print(data[i] >> 4, HEX);
        Serial.print(data[i] & 0x0F, HEX);
    }
    P1.TEMPERATURA=temperature(data,TEMP);
    // Outside/Water Temp : THN132N,...
    if(data[0] == 0xEA && data[1] == 0x4C)
    {
       Serial.print("[THN132N,...] Id:");
       Serial.print(data[3], HEX);
       Serial.print(" ,Channel:");
       Serial.print(channel(data));
       Serial.print(" ,temp:");
       Serial.print(P1.TEMPERATURA);
       Serial.print(" ,bat:");
       Serial.print(battery(data)); 
       Serial.println();
    }
    // Inside Temp-Hygro : THGR228N,...
    else if(data[0] == 0x1A && data[1] == 0x2D)
    {
       Serial.print("[THGR228N,...] Id:");
       Serial.print(data[3], HEX);
       Serial.print(" ,Channel:");
       Serial.print(channel(data));
       Serial.print(" ,temp:");
       Serial.print(P1.TEMPERATURA);
       Serial.print(" ,hum:");
       Serial.print(humidity(data));
       Serial.print(" ,bat:");
       Serial.print(battery(data)); 
       Serial.println();
    }
 
    decoder.resetDecoder();
    P1.teste=true;
    P1.ID=(data[3]);
    Serial.print("Variavel teste-->");
    Serial.println(P1.teste);
    Serial.print("Variavel temperatura-->");
    Serial.println(P1.TEMPERATURA);
    return P1;
}
