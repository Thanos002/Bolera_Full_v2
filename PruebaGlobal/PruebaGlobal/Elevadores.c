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


volatile int ERpos;
volatile int ECPos;
volatile int rutina=0;
volatile int Time=0;



//MOTORES
// EC: M1 di: PK6  en: PL0
// ER: M5 di: PD5  en: PD7
//SW
// EC: SW1: PD6
// ER: SW5: PD3 /INT3

//PULSADOR DIRSPARO
// SW6: PD2 INT2


//SET UP//



void setupEC(){
	setBit(M1_diDDR,M1_di_X);			//M1_diDDR=0x01
	setBit(M1_enDDR,M1_en_X);			//M1_enDDR=0x01;
	clearBit(SW1DDR,SW1X);				//SW1DDR=0x01;
	clearBit(M1_enPORT,M1_en_X);		//M1_enPORT=0;
}

void setupER(){
	setBit(M5_diDDR,M5_di_X);			//M5_diDDR=0x01;
	setBit(M5_enDDR,M5_en_X);			//M5_enDDR=0x01;
 	EICRA|=0x80;						//flanco de bajada
 	EIMSK|=0x08;						//INT3
	clearBit(M5_enPORT,M5_en_X);		//M5_enPORT=0;
}

void setupElevadores(){
	cli();
	setupEC();
	setupER();
	sei();
}



//FUNCIONES EC//


inline void bajaEC(){
	clearBit(M1_enPORT,M1_en_X);			// antes de cambiar dir es necesario deshabilitar el motor
	if(ECPos==1){
		clearBit(M1_diPORT,M1_di_X);		//M1_diPORT=0x00;
		setBit(M1_enPORT,M1_en_X);			//M1_enPORT=0x01;
		_delay_ms(1000);
		while((PIND & 0x40)!=0){}			//Espera a que vuelva a saltar SW
		clearBit(M1_enPORT,M1_en_X);		//M1_enPORT=0
		ECPos=0;
	}
}

inline void subeEC(){
	clearBit(M1_enPORT,M1_en_X);			// antes de cambiar dir es necesario deshabilitar el motor
	if(ECPos==0){
		setBit(M1_diPORT,M1_di_X);			//M1_diPORT=0;
		setBit(M1_enPORT,M1_en_X);			//M1_enPORT=1;
		_delay_ms(1000);					//Espera a que el SW1 se haya dejado de pulsar.
		while((PIND & 0x40)!=0){}			//Espera a que vuelva a saltar SW
		clearBit(M1_enPORT,M1_en_X);		//M1_enPORT=0
		ECPos=1;
	}
}

//FUNCIONES HOME

void homeEC(){
	clearBit(M1_enPORT,M1_en_X);		// antes de cambiar dir es necesario deshabilitar el motor
	setBit(M1_diPORT,M1_di_X);			//M1_diPORT=0x00; sube
	setBit(M1_enPORT,M1_en_X);			//M1_enPORT=0x01;
	_delay_ms(2000);					//Espera 2 segundos para asegurarse de que baja
	clearBit(M1_enPORT,M1_en_X);		//M1_enPORT=0
	ECPos=1;
}

void homeER(){
	//Coloca el motor en su posición inicial
	clearBit(M5_enPORT,M5_en_X);
	clearBit(M5_diPORT,M5_di_X);		//M5 di=1
	setBit(M5_enPORT,M5_en_X);			//M5 en=1
	_delay_ms(3000);					//Espera 3 segundos para asegurarse de que sube
	clearBit(M5_enPORT,M5_en_X);
	ERpos=0;

}

//FUNCIONES ER//


inline void bajaER(){
	if(ERpos==1){
				
setBit(EIFR,3);

		clearBit(M5_enPORT,M5_en_X);
		clearBit(M5_diPORT,M5_di_X);	//M5_diPORT=0x01;
		setBit(M5_enPORT,M5_en_X);
		
	
			
	}
}


inline void subeER(){
	if(ERpos==0){
				
	setBit(EIFR,3);
	
		clearBit(M5_enPORT,M5_en_X);
		setBit(M5_diPORT,M5_di_X);	//M5_diPORT=0x00;
		setBit(M5_enPORT,M5_en_X);
		
			

	}
}


//INTERRUPCIONES//


inline void OnSW5Interruption(){
		setBit(EIFR,3);
		clearBit(EIMSK,3);
		Time=0;
	if(rutina==0){
		if(readBit(M5_diPIN,M5_di_X)==0){	
			ERpos=0;
			clearBit(M5_enPORT,M5_en_X);
		}
		if(readBit(M5_diPIN,M5_di_X)==1){	
			ERpos=1;
			clearBit(M5_enPORT,M5_en_X);
		}
	}
	else if (rutina==1){			//Rutina de reacomodo
		ERpos=1;
		rutina=0;
		bajaER();
		
		}
			
	
	
}
inline void UpdateTimerElevadores(){
	Time=Time+5;
	if(Time>=500){
		setBit(EIMSK,3);
		setBit(EIFR,3);
	}
}



	//FUNCIONES COMPLEJAS//

inline void recarga(){
	
	rutina=1;
	subeER();
}





