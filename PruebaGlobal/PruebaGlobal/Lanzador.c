/*
* Lanzador.c
*
* Created: 19.04.2023 17:29:57
* Author : thano
*/

#include "IncFile1.h"
#include "Bolera.h"
#include "Lanzador.h"
#include "ConfiguracionV1.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile Position position = RIGHT;
volatile uint32_t ms_elapsed = 0;
volatile uint32_t last_interruption_time = 0;
volatile States state = HOME;

// para debugging
// #define _delay_ms(P) position=position;

// setup para el lanzador
void setupLanzador(){
	cli();
	enableInterrupt(SW2EIFR);  // set EIMSK for SW2 (INT0)
	EICRA |= (1<<ISC01); // interrupcion solo en flanco de bajada, no modificando otros valores del registro
	enableInterrupt(SW6EIFR);  //set EIMSK for SW6 (INT2)
	EICRA |= (1<<ISC21);
	// Sensores:
	setInput(SW2DDR, SW2X);
	setInput(SW3DDR, SW3X);
	setInput(SW4DDR, SW4X);
	setInput(SW5DDR, SW5X);
	// Motores:
	setOutput(M1_diDDR, M1_di_X);
	setOutput(M2_diDDR, M2_di_X);
	setOutput(M3_diDDR, M3_di_X);
	setOutput(M4_diDDR, M4_di_X);
	setOutput(M5_diDDR, M5_di_X);
	setOutput(M1_enDDR, M1_en_X);
	setOutput(M2_enDDR, M2_en_X);
	setOutput(M3_enDDR, M3_en_X);
	setOutput(M4_enDDR, M4_en_X);
	setOutput(M5_enDDR, M5_en_X);
	setOutput(M2_bkDDR, M2_bk_X);
	sei();
}

// HOME

// lanzador home: se mueve a la izquierda y se guarda el flag position
void lanzadorHome(){
	girarLanzador(0);
	_delay_ms(long_delay);
	pararLanzador();
	position = LEFT;
}

void vastagoHome(){
	moverVastagoAtras();
	_delay_ms(button_check_delay_ms);
	loop_until_bit_is_clear(SW3PIN,SW3X);
	pararVastago();
}

void carritoHome(){
	liberarCarrito();
	_delay_ms(button_check_delay_ms);
	loop_until_bit_is_clear(SW4PIN,SW4X);
	pararCarrito();
}

// funciones usadas para el timer de main
inline void updateTime(){
	ms_elapsed += 5;
	if(ms_elapsed - last_interruption_time<doublePressbuffer){  // comprobar si ha pasado el tiempo requisito para rehabilitar interrupciones SW2
		cli();
		disableInterrupt(SW2EIFR);
		sei();
	}
	else{
		cli();
		enableInterrupt(SW2EIFR);
		sei();
	}
}

// setter para el tiempo acual
inline void setTime(int time){
	ms_elapsed = time;
}
// getter para el valor del tiempo actual
inline uint32_t getTime(){
	return ms_elapsed;
}


// funcion que, si esta llamada, actualiza el estado de la led, para que esta parpadeando
// llamar la funcion mediante interrupciones temporales (resolucion <50ms)
inline void parpadearLED(){
	if(ms_elapsed % 200 <= 100){
		apagarLED();
	}
	else{
		encenderLED();
	}
}

// interrupcion para el boton de disparo
// incluye proteccion, se atiende solo cuando estamos en LANZAMIENTO

inline void OnSW2Interruption(){
	if(ms_elapsed-last_interruption_time>doublePressbuffer){  // doble chequeo, pero no es necesaio
		if(lanzadorFlag==1){
			last_interruption_time = ms_elapsed;
			girarLanzador(0);
		}
		else if(lanzadorFlag==0){
			last_interruption_time = ms_elapsed;
			girarLanzador(1);
		}
	}
}


