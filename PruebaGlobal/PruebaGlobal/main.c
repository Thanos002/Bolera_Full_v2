/*
 * PruebaGlobal.c
 *
 * Created: 19/05/2023 12:01:44
 * Author : mario
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include "ConfiguracionV1.h"
#include "IncFile1.h"
#include "Displays.h"
#include "Sensores_bolos.h"
#include "Lanzador.h"
#include "Bolera.h"
#include "Elevadores.h"

// para debugging
// #define _delay_ms(P) ultimoFlag=ultimoFlag;

// BANDERAS (evitar si posible)

volatile uint8_t parpadearFlag = 0;
volatile uint8_t ultimoFlag = 0;
volatile uint8_t habilitarEstadoFinal = 0;
volatile int state1,state2,state3,state4,state5,state6;
volatile int interrupt0 = 0;
volatile int interrupt2 = 0;

// Interrupciones:

ISR(TIMER0_COMPA_vect){						//Interrupcion que ocurre cada 5ms
	DisplayUpdater();  // update displays with high frecuency
	updateTime();		// update Lanzadores Timer
	if(parpadearFlag==1){
		parpadearLED();   // enable parpadeo if necessary
	}
	state1 = getSensor1();
	state2 = getSensor2();
	state3 = getSensor3();
	state4 = getSensor4();
	state5 = getSensor5();
	state6 = getSensor6();
}

ISR(TIMER1_COMPA_vect){  // cuando pasan los 30 segundos
	parpadearFlag = 1;
	ultimoFlag = 1;
	disableTimer1Int();
}

ISR(PCINT2_vect){ //Cuidado que era por flanco de bajada
	OnPinChangeBolos();
}

// interrupcon del SW2 que sirva para distinguir que pulsador se ha pulsado
ISR(INT0_vect){
	OnSW2Interruption();
	interrupt0 ++;
}

ISR(INT2_vect){  // interrupcion del pulsador del disparo (SW6)
	interrupt2 ++;
	if(state == LANZAMIENTO){
		habilitarInterrupcionesSensores();
		state = TIRAR_BOLA;
	}
	if(state == HOME){
		state = SIN_BOLA;
	}
	if(state==FINAL){
		state = HOME;
	}
}

ISR(INT3_vect) {  // interrupcion del sensor SW5 
	OnSW5Interruption();
}

int main(void)
{
	// proceso de setup de todos los componentes:
	_delay_ms(200);
	setup_puertos();
	setupLanzador();
	setup_timer0();
	setup_timer1();
	setupElevadores();
	homeER();
	// bajaER();  // comprobar si es necesario
	homeEC();
	// subeEC();  // comprobar si es necesario
	vastagoHome();
	carritoHome();
	lanzadorHome();  // comprobar si se puede hacer el home de manera segura (ELEVADOR)
    while (1) {
		switch(state){
			case HOME:  // despues de init
				ultimoFlag = 0;
				parpadearFlag = 0;
				// finalizadoFlag = 0; re resetea en LANZAMIENTO
				puntuacion = 0;
				// no hacemos nada, esperamos la interrupcion de disparo
				//loop_until_bit_is_set(SW6PIN,SW6X);
				//state = SIN_BOLA;
				break;
			case SIN_BOLA:
				subeEC();  // comprobar que EC esta en posicion alta
				// _delay_ms(long_delay);  sube bloquante
				girarLanzador(1);
				moverVastagoAtras();  // mover vastago atras para evitar que choca (ya esta)
				_delay_ms(long_delay);
				bajaEC();  // coger la bola
				// _delay_ms(long_delay);
				subeEC(); // subir la bola		
				// Aqui: verficiar que bola esta cargada:
				state = BOLA_LANZADOR;
				break;
			case BOLA_LANZADOR:
				// proceso para enganchar la bola en el lanzador

				// liberamos el carrito
				do {
					liberarCarrito();
				_delay_ms(button_check_delay_ms);}
				while(getSensor4()!=0);
				pararCarrito();
					
				// movemos el vastago adelante
				do {
					moverVastagoAdelante();
				_delay_ms(button_check_delay_ms);}
				while(getSensor3()!=0);
				pararVastago();
					
				// enganchamos el carrito
				do {
					engancharCarrito();
				_delay_ms(button_check_delay_ms);}
				while(getSensor4()!=0);
				pararCarrito();
					
				//movemos el vastago atras (enganchar gomas)
				do {
					moverVastagoAtras();
				_delay_ms(button_check_delay_ms);}
				while(getSensor3()!=0);
				pararVastago();
					
				_delay_ms(button_check_delay_ms);
					
				state = LANZAMIENTO; // empezar lanzamiento
				if(finalizadoFlag){
					resetTimer1();
					enableTimer1Int();
					finalizadoFlag = 0;
				}
				//buffer = getTime();  // timer de 30 segundos
				break;

			case LANZAMIENTO:
					
				// encender LED
				encenderLED();
					
				// despues de 30 seg. se habilita la bandera parpadear
				// Integracion: posiblemente sacarlo de los estados al nivel del main, asi se acualiza independientemente del estado
				
				//if(buffer+30000<getTime()){
				//	parpadear = 1;}
				
				girarLanzador(0);  // girar hacia la derecha
				_delay_ms(long_delay);
				
				// esperar interupciones
				// loop_until_bit_is_set(SW6PIN,SW6X);  //esperar hasta se pulsa el disparo
				// cuando se interumpe, marcar state=TIRAR_BOLA
				// state = TIRAR_BOLA;
				break;
					
			case TIRAR_BOLA:
				// asegurar que estamos a la izquierda del Lanzador:
				if(position==RIGHT){
					state = LANZAMIENTO;  // si no, procedemos con el lanzamiento
					girarLanzador(0);  // opcional: intentar corregir posicion
					break;
				}
					
				// El interruptor de disparo se ha pulsado
				parpadearFlag = 0;  // apagar led despues del disparo (y parpadeo)
				apagarLED();
				
				if(ultimoFlag==1){  // si ya se hizo la ultima tirada
					habilitarEstadoFinal = 1;  // pasar al estado final despues del retorno
				}
				
				frenoLanzador();  // frenamos el lanzador
				_delay_ms(button_check_delay_ms);
				liberarCarrito();  // tiramos la bola
				loop_until_bit_is_clear(SW4PIN,SW4X);
				pararCarrito();

				state = RETORNO;  // inicar estado de retorno
				break;
			case RETORNO:
				_delay_ms(4000);   // esperar hasta la bola llega
				recarga();
				_delay_ms(long_delay);
				if(habilitarEstadoFinal==1){ // si hemos tirado la bola y ya estamos en el ultimo disparo
					state = FINAL;  // volver a primer estado de espera
				}
				else{
					state = SIN_BOLA; // volver a recoger la bola 
				}
				break;
			case FINAL:
				finalizadoFlag = 1;  // parpadear display
				break;
			default:
				state = HOME;
				break;
				
		}
    }
}

