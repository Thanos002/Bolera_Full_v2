/*
 * IncFile1.h
 *
 * Created: 18/05/2023 17:28:51
 *  Author: mario
 */ 

#ifndef CONFIGURACIONV1_H
#define CONFIGURACIONV1_H

#ifndef F_CPU
#define F_CPU 8000000UL   // comprobar que la frecuencia es 8MHz y no 16MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setup_puertos();
// void setup_timer0();
void setup_timer1();

void resetTimer1();
void disableTimer1Int();
void enableTimer1Int();

extern volatile uint8_t puntuacion;

#endif /*CONFIGURACIONV1_H */