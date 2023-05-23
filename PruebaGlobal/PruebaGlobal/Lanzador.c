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
	engancharCarrito();
	_delay_ms(1000);
	liberarCarrito();
	_delay_ms(button_check_delay_ms);
	loop_until_bit_is_clear(SW4PIN,SW4X);
	pararCarrito();
}

// interrupcion del fin de carrera derecho,
inline void rightInterrupt(){
	if(state==LANZAMIENTO){  // si estamos en lanzamiento, cambiamos direccion
		girarLanzador(0);
	}
	else{
		pararLanzador();  // si no, paramos
	}
}

// interrupcion del pulsador medio
inline void middleInterrupt(){
	if(position==LEFT)	{
		position = RIGHT;  // cambiar position flag, que ahora estamos a la derecha
	}
	else{
		position = LEFT;
	}
	if(state==LANZAMIENTO){  // cambiar de direccion en estado de lanzamiento
		girarLanzador(0);
	}
}

// interrupcion del pulsador a la izquierda
inline void leftInterrupt(){
	if(state==LANZAMIENTO){
		girarLanzador(1);  // cambiar de direccion en lanzamiento
	}
	else{
		pararLanzador();
	}
}

// funciones usadas para el timer de main
inline void updateTime(){
	ms_elapsed += 5;
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
// debounce protection:
// si hay dos interupciones entre menos que 50 ms
	if(last_interruption_time +50 < ms_elapsed){
		last_interruption_time = ms_elapsed;  // guardar tiempo acual
		switch (lanzadorFlag){
			case 1:  // moviemiento hacia derecha
				// si estoy RIGHT
				if (position==RIGHT){
					rightInterrupt();
				}
				else if (position==LEFT){  // si estoy LEFT
					middleInterrupt();
				}
				break;
			case 0:  // moviendo hacia izq
				// si estoy moviendo hacia la izquierda y estoy LEFT
				if (position==LEFT){
					leftInterrupt();
				}
				// movimiento hacia la izqrda, estoy RIGHT
				else if (position==RIGHT){
					middleInterrupt();
				}
				break;
			// Puede haber interrupciones de bajada cuando estoy parado?
			case 2:  // parado
				if(position == LEFT){
					// leftInterrupt();
				}
				else{
					// rightInterrupt();
				}
				break;
		}
	}
}
// Explicacion de los estados:
// SIN_BOLA: la bola esta en el elevador de cargas, pero no en el lanzador
// BOLA_LANZADOR: la bola esta colocada en el lanzador
// LANZAMIENTO: el sistema se encuentra el el proceso de lanzar la bola
// TIRAR_BOLA: la bola fue lanzada para tirar los bolos



