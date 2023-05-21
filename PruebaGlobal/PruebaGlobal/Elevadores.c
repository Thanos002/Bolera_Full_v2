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

int ERpos;
int ECPos;
int rutina=0;
int current_time=0;
int bandera=0;
int disparador=0;

//MOTORES
// EC: M1 di: PK6  en: PL0
// ER: M5 di: PD5  en: PD7
//SW
// EC: SW1: PD6
// ER: SW5: PD3 /INT3

//PULSADOR DIRSPARO
// SW6: PD2 INT2


//SET UP//


// COMENTA QUE HACES!
void setuptimer(){
	TCCR0A= 0b00000010;
	TCCR0B=0b00000011;
	TIMSK0=0b00000010;
	OCR0A=124;
	TIFR0=0b00000010;
}

void setupEC(){
	setBit(M1_diDDR,M1_di_X);			//M1_diDDR=0x01
	setBit(M1_enDDR,M1_en_X);			//M1_enDDR=0x01;
	clearBit(SW1DDR,SW1X);		//SW1DDR=0x01;
	clearBit(M1_enPORT,M1_en_X);		//M1_enPORT=0;
}

void setupER(){
	setBit(M5_diDDR,M5_di_X);			//M5_diDDR=0x01;
	setBit(M5_enDDR,M5_en_X);			//M5_enDDR=0x01;
	clearBit(SW5DDR,SW5X);  //Comprobar!
	EICRA|=0x80;			//flanco de bajada
	EIMSK|=0x08;			//INT3
	EIFR|=0x08;
	clearBit(M5_enPORT,M5_en_X);		//M5_enPORT=0;
}

void setupPulsador() {
	clearBit(SW4DDR,SW4X);		//PD4 Pulsador de disparo
	EICRA|=0x20;			//flanco de bajada
	EIMSK|=0x04;			//INT3
	EIFR|=0x04;
}

void setupElevadores(){
	cli();
	setuptimer();
	setupEC();
	setupER();
	setupPulsador();
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
	clearBit(M1_enPORT,M1_en_X);  // antes de cambiar dir es necesario deshabilitar el motor
	if(ECPos==1){
		clearBit(M1_diPORT,M1_di_X);			//M1_diPORT=0x00;
		setBit(M1_enPORT,M1_en_X);			//M1_enPORT=0x01;
		_delay_ms(1000);
		while((PIND & 0x40)!=0){}	//Espera a que vuelva a saltar SW
		clearBit(M1_enPORT,M1_en_X);			//M1_enPORT=0
		ECPos=0;
	}
}

void subeEC(){
	clearBit(M1_enPORT,M1_en_X);  // antes de cambiar dir es necesario deshabilitar el motor
	if(ECPos==0){
		setBit(M1_diPORT,M1_di_X);			//M1_diPORT=0;
		setBit(M1_enPORT,M1_en_X);			//M1_enPORT=1;
		_delay_ms(1000);	//Espera a que el SW1 se haya dejado de pulsar.
		while((PIND & 0x40)==1){}	//Espera a que vuelva a saltar SW
		clearBit(M1_enPORT,M1_en_X);			//M1_enPORT=0
		ECPos=1;
	}
}

//FUNCIONES HOME

void homeEC(){
	clearBit(M1_enPORT,M1_en_X);  // antes de cambiar dir es necesario deshabilitar el motor
	setBit(M1_diPORT,M1_di_X);			//M1_diPORT=0x00; sube
	setBit(M1_enPORT,M1_en_X);			//M1_enPORT=0x01;
	_delay_ms(2000);					//Espera 2 segundos para asegurarse de que baja
	clearBit(M1_enPORT,M1_en_X);			//M1_enPORT=0
	ECPos=1;
}

void homeER(){
	//Coloca el motor en su posición inicial
	clearBit(M5_diPORT,M5_di_X);		//M5 di=1
	setBit(M1_enPORT,M5_en_X);		//M5 en=1
	_delay_ms(3000);				//Espera 4 segundos para asegurarse de que sube
	clearBit(M5_enPORT,M5_en_X);
	ERpos=0;
	bandera=0;
}

//FUNCIONES ER//


void bajaER(){
	if(ERpos==1){
		clearBit(M5_diPORT,M5_di_X);	//M5_diPORT=0x01;
		setBit(M5_enPORT,M5_en_X);	//M5_enPORT=0x01;
	}
}


void subeER(){
	if(ERpos==0){
		setBit(M5_diPORT,M5_di_X);	//M5_diPORT=0x00;
		setBit(M5_enPORT,M5_en_X);	//M5_enPORT=0x01;
	}
}
//Devuelve la posición de ER

int getEstadoER(){
	return ERpos;
}

////Parada ER sin interrupciones (por si acaso)
//
//int bajaERsin(){
//
//setBit(PORTD,5);//M5_diPORT=0x01;
//setBit(PORTD,7);//M5_enPORT=0x01;
//while(PIND3==0){}//Espera a que el SW se haya dejado de pulsar.
//while(PIND3==1){}//Espera a que vuelva a saltar SW
//clearBit(PORTD,7);//M1_enPORT=0
//return 0;
//}
//int subeERsin(){
//
//clearBit(PORTD,5);//M5_diPORT=0x00;
//setBit(PORTD,7);//M5_enPORT=0x01;
//while(PIND3==0){}//Espera a que el SW se haya dejado de pulsar.
//while(PIND3==1){}//Espera a que vuelva a saltar SW
//clearBit(PORTD,7);//M1_enPORT=0
//return 0;
//}



//int disparador(){return}

//INTERRUPCIONES//


//Cuando ER toca el tope nos dice donde está

// ESTA FUNCION NO ESTA AUTOEXPLICATIVA
inline void OnSW5Interruption(){
	/*if(bandera==1){*/
		if(readBit(M5_diPORT,M5_di_X)==0){	//M5 di= 1
			ERpos=0;
			clearBit(M5_enPORT,M5_en_X);
			bandera=0;
		}
		else if(readBit(M5_diPORT,M5_di_X)==1){	//M5 di= 0
			ERpos=1;
			clearBit(M5_enPORT,M5_en_X);
			bandera=0;
		}
		//if (rutina==1){			//Rutina de reacomodo
		//if(ERpos==1){
		//bajaER();
		//rutina=0;
		//bandera=0;
		//}
		
		
		//}

		// 	}
		// 		else{bandera=1;}
	}

inline void UpdateTimeElevadores(){
		current_time += 5;
	}

	//FUNCION DISPARADOR

// NO SE USA
int disparo(){
	if(disparador==1){
		disparador=0;
		return 1;
	}
	else{
		return 0;
	}
}

	//FUNCIONES COMPLEJAS//

// NO SE USA, SE USA LA MAQUINA DE ESTADOS A SU VEZ
int recarga(int td,int ultimo_disparo){//td es el tiempo de delay y ultimo disparo indica si se ha realizado el disparo posterior a 30s
	_delay_ms(4000);  // td fijo necesario
	rutina=1;
	subeER();
	if(ultimo_disparo==1){
		while((PIND & 0x10)!=1){} //PD4 Pulsador de disparo
	}
	return 0;
}


