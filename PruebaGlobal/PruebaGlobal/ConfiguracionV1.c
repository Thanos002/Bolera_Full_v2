/*
 * Configuracion.c
 *
 * Created: 18/05/2023 12:48:03
 * Author : mario
 */ 

#include "ConfiguracionV1.h"
#include "IncFile1.h"

// controla PCINTS para sensores de los bolos como entradas y además las señales 'direccion' del Motor M1 y M2 que son salidas. Comprobar si necesitan sus interrupciones activadas o no.
void setup_puertos(){  // solo para Sensores Opticos y Displays
cli();
//Puerto K						
	//DDRK = 0xC0;				//0b11000000
	setBit(M2_diDDR,M2_di_X);
	setBit(M1_diDDR,M1_di_X);
	//PCICR = (1<<PCIE2);			// Para activar las PCINT globalmente del grupo 3 (PCINT16-PCINT23)
	setBit(PCICR,PCIE2);
	//PCMSK2 = 0x3F;				//0b00111111 (Habilitar localmente PCINT16-PCINT21)Hay que habilitarlos cuando llegue la señal de disparo, pq durante las deshabilitamos
// 	setBit(PCMSK2,5);
// 	setBit(PCMSK2,4);
// 	setBit(PCMSK2,3);				//En teoría esto se habilita luego
// 	setBit(PCMSK2,2);
// 	setBit(PCMSK2,1);
// 	setBit(PCMSK2,0);
	
//Puerto B						controla display de 7 segmentos y el selector de display. Se trata como salidas.
	//DDRB = 0xFF;				//Puerto B configurado como salidas para controlar display 7 segmentos y selector
	setBit(DaDDR,DaX);
	setBit(DbDDR,DbX);
	setBit(DcDDR,DcX);
	setBit(DdDDR,DdX);
	setBit(DeDDR,DeX);
	setBit(DfDDR,DfX);
	setBit(DgDDR,DgX);  // Thanos: cambiado DfX por dgX
	setBit(DSDDR,DSX);
	//PORTB = 0b00111111;			//Inicialemente display muestra 0 en el display 0
	setBit(DfPORT,DfX);							//Puerto B tiene PCINT0-PCINT7 pero no los utilizamos.
	setBit(DePORT,DeX);	
	setBit(DdPORT,DdX);	
	setBit(DcPORT,DcX);	
	setBit(DbPORT,DbX);
	setBit(DaPORT,DaX);		
//Puerto D						dedicado a los fines de carrera como entradas SW y a manejar el motor M5, tratado como salidas.
	//DDRD = 0b10100000;			//PD7 y PD5 son salidas para el motor M5, el resto entradas para los SWi.
	setBit(M5_enDDR,M5_en_X);
	setBit(M5_diDDR,M5_di_X);
	//EICRA = 0xAA;				//0b10101010Todas las int por flanco de bajada 	//Puerto D tiene INT0-INT3 en sus cuatro primeros bits
	//setBit(EICRA,7);			// deshabilitado por problemas INT3 (Thanos 25/05)
	setBit(EICRA,5);
	/*setBit(EICRA,3);*/	 //Nueva actualziacion
	setBit(EICRA,1);
	//EIMSK = 0x0F;				//Habilitar interrupciones INT0-INT3
	enableInterrupt(SW2EIFR);
	//enableInterrupt(SW5EIFR);  // deshabilitado por problemas (Thanos 25/05)
	//setBit(EIMSK,1);         Thanos no usaba interrupciones para sensor vástago
	enableInterrupt(SW6EIFR);
//Puerto L						dedicado al manejo de los motores y al LED del sistema lanzador. Todo salidas
	//DDRL = 0xFF;	
	setBit(M1_enDDR,M1_en_X);
	setBit(M2_enDDR,M2_en_X);
	setBit(M3_diDDR,M3_di_X);
	setBit(M2_bkDDR,M2_bk_X);
	setBit(M3_enDDR,M3_en_X);
	setBit(M4_diDDR,M4_di_X);
	setBit(L1DDR,L1X);
	setBit(M4_enDDR,M4_en_X);
	
sei();
}


// La funcion setup_timer() esta en el fichero Assembly2.s en ensamblador
/*

void setup_timer0(){			//Preparado para interrumpir cada 5 ms
	cli();
	//TCCR0A = 0b00000010;		//Modo CTC
	setBit(TCCR0A,WGM01);
	//TCCR0B = 0b00000100;		//Prescalado 256
	setBit(TCCR0B,CS02);
	//TIMSK0 = 0b00000010;		//Habilita OCR0A 
	setBit(TIMSK0,OCIE0A);
	OCR0A =  156;				//Para contar cada 5ms
	sei();
}	
*/

// funciones para controlar el Timer1 desde la main().

inline void resetTimer1(){
	TCNT1 = 0x00;
}

inline void disableTimer1Int(){
	TIMSK1 = 0x00;
}

inline void enableTimer1Int(){
	TIMSK1 = 0b00000010;
}

void setup_timer1(){			//Preparado para interrumpir cada 3 seg
	cli();
	//TCCR1A = 0b00000000;
	//TCCR1B = 0b00001101;		//Prescalado 1024
	setBit(TCCR1B,CS10);
	setBit(TCCR1B,CS12);
	setBit(TCCR1B,WGM12);
	//TIMSK1 = (1<<OCIE1A);
	setBit(TIMSK1,OCIE1A);
	OCR1A  = 23437;				// timer para interrumpir cada 3 segundos
	sei();
}
	




