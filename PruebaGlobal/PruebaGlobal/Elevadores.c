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
#include <avr/sfr_defs.h>

#include "IncFile1.h"
#include "Elevadores.h"

volatile uint8_t ERpos;
volatile uint8_t ECPos;
volatile uint8_t rutina=0;
volatile uint8_t current_time=0;


// para debugging
// #define _delay_ms(P) rutina=rutina;


//MOTORES
// EC: M1 di: PK6  en: PL0
// ER: M5 di: PD5  en: PD7
//SW
// EC: SW1: PD6
// ER: SW5: PD3 /INT3

//PULSADOR DIRSPARO
// SW6: PD2 INT2

// rutina 0: cuando llega al fin se para
// rutina 1: cuando llega arriba baja

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



//Rutina de espera
// REEMPLAZADA POR _DELAY_MS PARA MAYOR EFICIENCIA
/*
void wait(int t){
	volatile int t0=current_time;
	while( current_time-t0<t){}
}
*/  

//FUNCIONES EC//


void bajaEC(){
	clearBit(M1_enPORT,M1_en_X);			// antes de cambiar dir es necesario deshabilitar el motor
	if(ECPos==1){
		clearBit(M1_diPORT,M1_di_X);		//M1_diPORT=0x00;
		setBit(M1_enPORT,M1_en_X);			//M1_enPORT=0x01;
		_delay_ms(1000);
		loop_until_bit_is_clear(SW1PIN,SW1X);
		// while((PIND & 0x40)!=0){}			//Espera a que vuelva a saltar SW
		clearBit(M1_enPORT,M1_en_X);		//M1_enPORT=0
		ECPos=0;
	}
}

void subeEC(){
	clearBit(M1_enPORT,M1_en_X);			// antes de cambiar dir es necesario deshabilitar el motor
	if(ECPos==0){
		setBit(M1_diPORT,M1_di_X);			//M1_diPORT=0;
		setBit(M1_enPORT,M1_en_X);			//M1_enPORT=1;
		_delay_ms(1000);					//Espera a que el SW1 se haya dejado de pulsar.
		loop_until_bit_is_clear(SW1PIN,SW1X);
		// while((PIND & 0x40)==1){}			//Espera a que vuelva a saltar SW
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


void bajaER(){
	if(ERpos==1){
		clearBit(M5_enPORT,M5_en_X);
		clearBit(M5_diPORT,M5_di_X);	//M5_diPORT=0x01;
		setBit(M5_enPORT,M5_en_X);		//M5_enPORT=0x01;
	}
}


void subeER(){
	if(ERpos==0){
		clearBit(M5_enPORT,M5_en_X);
		setBit(M5_diPORT,M5_di_X);	//M5_diPORT=0x00;
		setBit(M5_enPORT,M5_en_X);	//M5_enPORT=0x01;
	}
}
//Devuelve la posición de ER

int getEstadoER(){
	return ERpos;
}




//INTERRUPCIONES//


inline void OnSW5Interruption(){
	if(rutina==0){
		if(readBit(M5_diPORT,M5_di_X)==0){	
			ERpos=0;
			clearBit(M5_enPORT,M5_en_X);
		}
		if(readBit(M5_diPORT,M5_di_X)==1){	
			ERpos=1;
			clearBit(M5_enPORT,M5_en_X);
		}
	}
	if (rutina==1){			//Rutina de reacomodo
		ERpos = 1;
		rutina=0;
		bajaER();
	}
}



	//FUNCIONES COMPLEJAS//

void recarga(){ 
	rutina=1;
	subeER();
}



/*
int main(void)
{
	setupElevadores();
	
	homeER();
	//homeEC();
	
	subeER();
	_delay_ms(2000);
	bajaER();

	
     while (1) 
	
    {
	
    }
}
*/
