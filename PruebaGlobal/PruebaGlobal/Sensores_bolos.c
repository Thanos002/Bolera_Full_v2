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
#include <avr/interrupt.h>

// uint8_t puntuacion = 0;  // Comentario Thanos: ojo, doble declarcion de la variable en Displays.c

/*Ya hecho en ConfiguracionV1*/
// void setup_Sensores(){
// 	cli();
// 	DDRK = 0xC0;           // 0b11000000 (SO1-S05 como entradas) (Bolos)
// 	PCICR = (1<<PCIE2);    // Habilitamos el PCIE2 porque vamos a usar PCINT16-PCINT21 (estan en PCMSK2 -> PCIE2) (Bolos)
// 	PCMSK2 = 0x3F;         // 0b00111111 Hay que habilitarlos cuando llegue la señal de disparo, pq durante las deshabilitamos (Bolos)
// 	sei();
// }

void habilitarInterrupcionesSensores(){
	SOPCMSK = 0x3F;
}

void deshabilitarInterrupcionesSensores(){
	SOPCMSK = 0x00;
}

uint8_t getPuntacion(){
	return puntuacion;
}

void setPuntuacion(uint8_t num){
	puntuacion = num;
}


inline void OnPinChangeBolos(){
	puntuacion++;
	uint8_t puertoactual = ~PINK;  //PINK normalmente estará a 1 !!
	//Lo negamos para que al tocar (flanco de bajada,0, entonces en puertoactual 1)
	//AND bit a bit para determinar si un bit en particular ha cambiado en el puerto
	if(puertoactual & (1<<SO1PIN)){
		//PCMSK2 &=~(1<<PCINT16);
		clearBit(SOPCMSK,PCINT16);
	} //Deshabilito esa interrupcion para que no rebote
	if(puertoactual & (1<<SO2PIN)){
		//PCMSK2 &=~(1<<PCINT17);
		clearBit(SOPCMSK,PCINT17);
	}
	if(puertoactual & (1<<SO3PIN)){
		//PCMSK2 &=~(1<<PCINT18);
		clearBit(SOPCMSK,PCINT18);
	}
	if(puertoactual & (1<<SO4PIN)){
		//PCMSK2 &=~(1<<PCINT19);
		clearBit(SOPCMSK,PCINT19);
	}
	if(puertoactual & (1<<SO5PIN)){
		//PCMSK2 &=~(1<<PCINT20);
		clearBit(SOPCMSK,PCINT20);
	}
	if(puertoactual & (1<<SO6PIN)){
		//PCMSK2 &=~(1<<PCINT21);
		clearBit(SOPCMSK,PCINT21);
	}
	//ese and del if entra siempre que en 1 de los 8 bits salga un 1
	//	puertoanterior=puertoactual;
}




