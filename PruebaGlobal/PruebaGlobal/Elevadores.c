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
 	// EICRA|=0x80;						//SW5 interrumpe por flanco de bajada.
 	// EIMSK|=0x08;						//Habilita INT3(SW5).
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
	_delay_ms(3000);					//Esperamos para asegurar que baja.
	clearBit(M5_enPORT,M5_en_X);		//M5_enable off.
	ERpos=0;							//ER está abajo

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

// no se usa
inline void recarga(){								//El elevador sube y cuando llegue arriba bajará.
	rutina=1;									//Bandera para que baje al llegar arriba.
	subeER();									//Empieza a subir.
}


//INTERRUPCIONES//
// no se usa
inline void OnSW5Interruption(){						//Interrupción para gestionar las paradas del ER.
		cli();
		clearBit(EIMSK,3);						//Deshabilitamos la interrupción.
		sei();
		//setBit(EIFR,3);							//Limpiamos las banderas de la interrupción.
		Time=0;									//Despues de un tiempo la volveremos a habilitar (antirrebotes).
		if(rutina==0){							//Si queremos que pare.
			if(readBit(M5_diPIN,M5_di_X)==0){	//Si estaba bajando.
				ERpos=0;						//Está abajo.
				clearBit(M5_enPORT,M5_en_X);	//M5_enable off.
			}
			if(readBit(M5_diPIN,M5_di_X)==1){	//Si estaba subiendo.
				ERpos=1;						//Está arriba.
				clearBit(M5_enPORT,M5_en_X);	//M5_enable off.
			}
		}
		else if (rutina==1){					//Si queremos que baje en lugar de que pare.
			ERpos=1;							//Está arriba.
			rutina=0;							//Bajamos la bandera.
			bajaER();							//Empieza a bajar.
		}
}
// no se usa
inline void UpdateTimerElevadores(){				//Rehabilita la interrupción de SW5(antirrebotes).
	Time=Time+5;							//Salta cada 5ms.
	if(Time>=20){							//Si ha pasado el tiempo de espera.
		cli();
		setBit(EIMSK,3);					//Habilita la interrupción
		sei();
		//setBit(EIFR,3);						//Limpia la bandera.
	}
}









