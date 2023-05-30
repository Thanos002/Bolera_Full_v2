/*
 * Sensores_bolos.h
 *
 * Created: 18/05/2023 13:13:45
 *  Author: mrjav
 * 
 */ 


#ifndef SENSORES_BOLOS_H_
#define SENSORES_BOLOS_H_

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototipos de función

void setup_Sensores();

void habilitarInterrupcionesSensores();
void deshabilitarInterrupcionesSensores();
void OnPinChangeBolos();
// extern volatile uint8_t puntuacion;

uint8_t getPuntacion();
void setPuntuacion(uint8_t num);

#endif /* SENSORES_BOLOS_H_ */