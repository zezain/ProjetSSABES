// Oregon V2 decoder modfied - Olivier Lebrun
// Oregon V2 decoder added - Dominique Pierre
// New code to decode OOK signals from weather sensors, etc.
// 2010-04-11 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: ookDecoder.pde 5331 2010-04-17 10:45:17Z jcw $
 //Libraries necessaires
#include <Wire.h>                                 //bibliotèque du protocole I2C
//Definition de constantes
#define MASQUE 0xFF
#define MQTT_SERVER "192.168.0.101"
#define MQTT_SERVER_SUPELEC "172.17.3.7"//"172.17.3.20"

//Bibliotèques du shield ethernet, choisir les bibliothèues utiles
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetClient.h>
#include <PubSubClient.h>
#include "Arduino.h"

#include "OregonDecoderV2.h"
#include "AffichageOregon.h"
#include "OregonDecoder.h"
#include "Stage.h"




// --- DEFINITIONS RESEAU ---//

byte mac[] = {  0x90, 0xA2, 0xDA, 0x10, 0x67, 0x4D }; //90-A2-DA-0F-15-15
IPAddress ip(172,17,3,224);                        //192.168.1.62 à choisir sur votre réseau local 172,17,3,224  192,168,0,15
EthernetClient client;                              //Initialise le client Ethernet
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
 

PubSubClient clientmqtt(MQTT_SERVER_SUPELEC, 1883, callback, client);


// --- VARIABLES DU PROGRAMME ---//
unsigned long temps=0;                             //Variable pour récuperer le temps
unsigned int SUM=0;                                //Variables mises a jour de temps en temps
unsigned int KWH=0;  
volatile word pulse;
boolean teste= false;
float TEMP;

//RGB LED pins
int ledDigitalOne[] = {10, 11, 9}; //the three digital pins of the digital LED 
                                   //10 = redPin, 11 = greenPin, 9 = bluePin
int relayDigitalPin=6;

int state=1;

boolean etat;
boolean dernierDebordement;

OregonDecoderV2 orscV2;

AffichageOregon afficheur;

Permission P1;

void ext_int_1()
{
    static word last;
    // determine the pulse length in microseconds, for either polarity
    pulse = micros() - last;
    last += pulse;
}


void commandeRelay(float temp)
{
    if(temp<tMin)
    {
        etat=HIGH;
        dernierDebordement=LOW;
        digitalWrite(relayDigitalPin,etat);
    }
    else if (temp>tMax)
    {
        etat=LOW;
        dernierDebordement=HIGH;
        digitalWrite(relayDigitalPin,etat);
    }
}

void setColor(int* led, boolean* color){
 for(int i = 0; i < 3; i++){
   digitalWrite(led[i], color[i]);
 }
}

/* A version of setColor that allows for using const boolean colors
*/
void setColor(int* led, const boolean* color){
  boolean tempColor[] = {color[0], color[1], color[2]};
  setColor(led, tempColor);
}

void indicaLed(float temp)
{
    if (temp<tMax && temp >tMin) setColor(ledDigitalOne,COLORS[1]); else setColor(ledDigitalOne,COLORS[0]);
}

//  -------- FONCTION QUI APPELE L'ESCLAVE ----------//
void appelEsclave(int add)
{
  byte a,b;                                        // On doit créer deux variables locales vu que l'on sait la taille de l'informations trasmise
   Wire.requestFrom(add, 2);                         // int - 2 octets pour l'esclave d'addresse #8
   while(Wire.available())                         // slave may send less than requested
  { 
    Serial.println("Il a fait l'appel");
    a = Wire.read();                               // receive a byte as character
    b = Wire.read();
    KWH=a<<8 | b;
    SUM=SUM+KWH;

    //Affichage serial
    Serial.println("Donnee Recue"); 
    Serial.println(KWH);
    Serial.println(SUM);
    Serial.println(temps);
    Serial.println(temps%100==0);
   }
}


void setup ()
{
    Ethernet.begin(mac, ip);                          //Démarer la connexion Ethernet et le serveur
    Wire.begin();                                     // join i2c bus (address optional for master)
    Serial.begin(115200);
    for(int i = 0; i < 3; i++){
    pinMode(ledDigitalOne[i], OUTPUT);   //Set the three LED pins as outputs
    }
    pinMode(relayDigitalPin,OUTPUT);
    Serial.println("\n[ookDecoder]\n");
    //Serial.println(sizeof(orscV2.DONE));
    //Serial.println(orscV2.add,HEX);
    attachInterrupt(0,ext_int_1, CHANGE);
    //DDRE  &= ~_BV(PE5); //input with pull-up 
    //PORTE &= ~_BV(PE5);

    //setColor(ledDigitalOne,COLORS[0]); 
}
 

void loop () {
    static int i = 0;
    cli();                      //Masquage d'interruption, p ne peut pas etre change
    word p = pulse;             //p doit etre forcement du même type que pulse <<word>>
 
    pulse = 0;
    sei();                      //Habilitations des interruptions
 
    if (p != 0)
    {
        if (orscV2.nextPulse(p))
            P1=afficheur.reportSerial("OSV2", orscV2,TEMP);
            // TEMP=afficheur.temperature(data,TEMP);
            //teste=true;

            if (P1.teste==true && P1.ID==0xF3){
            appelEsclave(8);
            String DON1=String(SUM);
            String DON2=String(P1.TEMPERATURA);
            //Serial.print(DON1);
            //Serial.print("Tamanho de SUm-->");
            //Serial.println(sizeof(DON1));
            char b[sizeof(DON1)];
            DON1.toCharArray(b,sizeof(DON1));
            char c[sizeof(DON2)];
            DON2.toCharArray(c,sizeof(DON2));
            
            String test =String('E'+DON1+'T'+DON2);
            //Serial.println(test);

            char d[test.length()+1];

            test.toCharArray(d,test.length()+1);
            //Serial.println(test.length()+1);
            Serial.println(d);
            
            if (clientmqtt.connect("arduinoClient","252",2,true,d)) {
                clientmqtt.publish("teste",b);
                //clientmqtt.publish("251",d);  
                Serial.println("MQTT OK");
                commandeRelay(P1.TEMPERATURA);
                indicaLed(P1.TEMPERATURA);
                P1.teste=false;           
              }



            else Serial.println("Erreur MQTT");
            P1.teste=false;
            }
    // MQTT client loop processing
    clientmqtt.loop();     
    }
    
    if (client.connected()) {
        client.stop();                             // DISCONNECT FROM THE SERVER
    }

    //teste=false;
}
