/*
* Sensores_bolos.c
*
* Created: 18/05/2023 13:12:59
* Author : mrjav
*/

#include "Sensores_bolos.h"
#include "ConfiguracionV1.h"
#include "IncFile1.h"

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

// uint8_t puntuacion = 0;  // Comentario Thanos: ojo, doble declarcion de la variable en Displays.c

/*Ya hecho en ConfiguracionV1*/
void setup_Sensores(){
	cli();
	DDRK = 0xC0;           // 0b11000000 (SO1-S05 como entradas) (Bolos)
	PCICR = (1<<PCIE2);    // Habilitamos el PCIE2 porque vamos a usar PCINT16-PCINT21 (estan en PCMSK2 -> PCIE2) (Bolos)
	PCMSK2 = 0x3F;         // 0b00111111 Hay que habilitarlos cuando llegue la señal de disparo, pq durante las deshabilitamos (Bolos)
	sei();
}

void habilitarInterrupcionesSensores(){
	PCMSK2 = 0x3F;
}

void deshabilitarInterrupcionesSensores(){
	PCMSK2 = 0x00;
}


inline void OnPinChangeBolos(){
	puntuacion++;
	uint8_t puertoactual = ~PINK;  //PINK normalmente estará a 1 !!
	//volatile uint8_t bitscambiados= 0;
	//	bitscambiados = ~(puertoactual ^ puertoanterior); //Lo negamos por el tema de que esta en nivel alto y pasa a nivel bajo
	
	//AND bit a bit para determinar si un bit en particular ha cambiado en el puerto
	if(puertoactual & (0b00000001)){
		PCMSK2 &=~(1<<PCINT16);
	} //Deshabilito esa interrupcion para que no rebote
	if(puertoactual & (0b00000010)){
		PCMSK2 &=~(1<<PCINT17);
	}
	if(puertoactual & (0b00000100)){
		PCMSK2 &=~(1<<PCINT18);
	}
	if(puertoactual & (0b00001000)){
		PCMSK2 &=~(1<<PCINT19);
	}
	if(puertoactual & (0b00010000)){
		PCMSK2 &=~(1<<PCINT20);
	}
	if(puertoactual & (0b00100000)){
		PCMSK2 &=~(1<<PCINT21);
	}
	//ese and del if entra siempre que en 1 de los 8 bits salga un 1
	//	puertoanterior=puertoactual;
}




