/*
 * Displays.h
 *
 * Created: 18/05/2023 13:01:14
 *  Author: mrjav
 */ 


#ifndef DISPLAYS_H_
#define DISPLAYS_H_

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototipos de función

void setup_Displays();
void apagarDisplay();
void setDisplay(int numero);

void DisplayUpdater();

#endif /* DISPLAYS_H_ */