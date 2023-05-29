/*
* Elevadores.c
*
* Created: 09/05/2023 13:24:24
* Author : iagol
*/

#ifndef F_CPU
#define F_CPU 8000000UL   // comprobar que la frecuencia es 8MHz y no 16MHz
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "IncFile1.h"
#include "Elevadores.h"


volatile uint8_t ERpos;		//Posición del Elevador de Retorno(ER).
volatile uint8_t ECPos;		//Posición del Elevador de Carga(EC).
volatile uint8_t rutina=0;	//Bandera para subida y bajada del ER.
volatile uint16_t Time=0;	//Variable de tiempo



//SET UP//

void setupEC(){
	setBit(M1_diDDR,M1_di_X);			//M1_di como salida.
	setBit(M1_enDDR,M1_en_X);			//M1_en como salida.
	clearBit(SW1DDR,SW1X);				//SW1 como entrada.
	clearBit(M1_enPORT,M1_en_X);		//M1_enable off.
}

void setupER(){
	setBit(M5_diDDR,M5_di_X);			//M5_di como salida.
	setBit(M5_enDDR,M5_en_X);			//M5_en como salida.
	clearBit(M5_enPORT,M5_en_X);		//M5_enable off.
}

void setupElevadores(){  //Realizamos todo el setup.
	cli();
	setupEC();
	setupER();
	sei();
}

//FUNCIONES HOME

void homeEC(){							//Lleva el EC a su posición inicial (arriba).
	clearBit(M1_enPORT,M1_en_X);		//M1_enable off.
	setBit(M1_diPORT,M1_di_X);			//Dirección ascendente (1).
	setBit(M1_enPORT,M1_en_X);			//M1_enable on.
	_delay_ms(2000);					//Esperamos para asegurar que sube. 
	clearBit(M1_enPORT,M1_en_X);		//M1_enable off.
	ECPos=1;							//EC está arriba.
}

void homeER(){							//Lleva el ER a su posición inicial (abajo).
	clearBit(M5_enPORT,M5_en_X);		//M5_enable off.
	clearBit(M5_diPORT,M5_di_X);		//Dirección descendente (0);
	setBit(M5_enPORT,M5_en_X);			//M5_enable on.
}


//FUNCIONES EC//

inline void bajaEC(){									//Bajamos el EC y bloqueamos el programa hasta que llegue a abajo.
	clearBit(M1_enPORT,M1_en_X);				//M5_enable off.
		if(ECPos==1){							//Solo se ejecuta si el EC está arriba.
			clearBit(M1_diPORT,M1_di_X);		//Dirección descendente (0);
			setBit(M1_enPORT,M1_en_X);			//M1_enable on.
			while((PIND & 0x40)==0){};			//Esperamos a que suelte el SW1.(Probar con while((PIND & 0x40)==0){} )
			while((PIND & 0x40)!=0){};			//Esperamos a que vuelva a pulsar SW1.
			clearBit(M1_enPORT,M1_en_X);		//M1_enable off.
			ECPos=0;							//EC está abajo.
		}
}
inline void subeEC(){									//Subimos el EC y bloqueamos el programa hasta que llegue arriba.
	clearBit(M1_enPORT,M1_en_X);				//M5_enable off.
		if(ECPos==0){							//Solo se ejecuta si el EC está abajo.
			setBit(M1_diPORT,M1_di_X);			//Dirección ascendente (1);
			setBit(M1_enPORT,M1_en_X);			//M1_enable on.
			while((PIND & 0x40)==0){};			//Esperamos a que suelte el SW1.(Probar con while((PIND & 0x40)==0){} )
			while((PIND & 0x40)!=0){};			//Esperamos a que vuelva a pulsar SW1.
			clearBit(M1_enPORT,M1_en_X);		//M1_enable off.
			ECPos=1;							//EC está arriba.
		}
}



//FUNCIONES ER//


inline void bajaER(){
	//if(ERpos==1){								//Bajamos el ER sin bloquear el programa.
		clearBit(M5_enPORT,M5_en_X);			//M5_enable off.
		clearBit(M5_diPORT,M5_di_X);			//Dirección descendente (0).
		setBit(M5_enPORT,M5_en_X);				//M5_enable on.
	//}
}


inline void subeER(){
	//if(ERpos==0){								//Subimos el ER sin bloquear el programa.
		clearBit(M5_enPORT,M5_en_X);			//M5_enable off.
		setBit(M5_diPORT,M5_di_X);				//Dirección ascendente	(1);
		setBit(M5_enPORT,M5_en_X);				//M5_enable on.
	//}
}


