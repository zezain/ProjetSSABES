/*
	Stage.h-Biblioteca do stagio

	// Breno Zaidan Martinelli

*/

#ifndef _STAGE_H
#define _STAGE_H

const float tMax=28.5;
const float tMin=27.5;
const float tRef=28.0;
//const boolean typesDebordement[]={LOW,HIGH}

const boolean ON = HIGH;     //Define on as LOW (this is because we use a common 
                            //Anode RGB LED (common pin is connected to +5 volts)
const boolean OFF = LOW;   //Define off as HIGH

//Predefined Colors
const boolean RED[] = {ON, OFF, OFF};    
const boolean GREEN[] = {OFF, OFF, ON};
const boolean BLUE[] = {OFF, ON, OFF};
const boolean YELLOW[] = {ON,OFF,ON}; 
const boolean CYAN[] = {OFF, ON, ON}; 
const boolean MAGENTA[] = {ON,ON,OFF}; 
const boolean WHITE[] = {ON, ON, ON}; 
const boolean BLACK[] = {OFF, OFF, OFF}; 
//boolean* COLORS[8];
//An Array that stores the predefined colors (allows us to later randomly display a color)
const boolean* COLORS[] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, BLACK};

#endif