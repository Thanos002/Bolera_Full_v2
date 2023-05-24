/*
 * Configuracion.c
 *
 * Created: 18/05/2023 12:48:03
 * Author : mario
 */ 

#include "ConfiguracionV1.h"
#include "IncFile1.h"

void setup_puertos(){  // solo para Sensores Opticos y Displays
cli();
//Puerto K						controla PCINTS para sensores de los bolos como entradas y además las señales 'direccion' del Motor M1 y M2 que son salidas. Comprobar si necesitan sus interrupciones activadas o no.		SensoresBolos.c
	DDRK = 0xC0;				//0b11000000
	PCICR = (1<<PCIE2);			// Para activar las PCINT globalmente del grupo 3 (PCINT16-PCINT23)
	PCMSK2 = 0x3F;				//0b00111111 (Habilitar localmente PCINT16-PCINT21)Hay que habilitarlos cuando llegue la señal de disparo, pq durante las deshabilitamos
	
//Puerto B						controla display de 7 segmentos y el selector de display. Se trata como salidas.
	DDRB = 0xFF;				//Puerto B configurado como salidas para controlar display 7 segmentos y selector
	PORTB = 0b00111111;			//Inicialemente display muestra 0 en el display 0
								//Puerto B tiene PCINT0-PCINT7 pero no los utilizamos.
	
//Puerto D						dedicado a los fines de carrera como entradas SW y a manejar el motor M5, tratado como salidas.
	DDRD = 0b10100000;			//PD7 y PD5 son salidas para el motor M5, el resto entradas para los SWi.
	EICRA = 0xAA;				//0b10101010Todas las int por flanco de bajada 	//Puerto D tiene INT0-INT3 en sus cuatro primeros bits
	//EIMSK = 0x0F;				//Habilitar interrupciones INT0-INT3
	enableInterrupt(SW2EIFR);
	enableInterrupt(SW5EIFR);
	enableInterrupt(SW6EIFR);
	
//Puerto L						dedicado al manejo de los motores y al LED del sistema lanzador. Todo salidas
	DDRL = 0xFF;	
	
sei();
}

void setup_timer0(){			//Preparado para interrumpir cada 5 ms
	cli();
	TCCR0A = 0b00000010;		//Modo CTC
	TCCR0B = 0b00000100;		//Prescalado 256
	TIMSK0 = 0b00000010;		//Habilita OCR0A 
	OCR0A =  156;				//Para contar cada 5ms
	
	sei();
}	

inline void resetTimer1(){
	TCNT1 = 0x00;
}

inline void disableTimer1Int(){
	TIMSK1 = 0x00;
}

inline void enableTimer1Int(){
	TIMSK1 = 0b00000010;
}

void setup_timer1(){			//Preparado para interrumpir cada 30 seg
	cli();
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001101;
	TIMSK1 = (1<<OCIE1A);
	OCR1A  = 23436;				// timer para interrumpir cada 3 segundos
	sei();
}
	




