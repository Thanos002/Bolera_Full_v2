/*
 * main.c
 *
 * Created: 19/05/2023 12:01:44
 * Author : Grupo 4 Bolera
 * Contributors: Iago, Ricardo, Belen, Javi, Mario, Thanos
 * Version: 0.14.2
 * Last modified: 30.05.2023
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
extern void setup_timer0();  // marcar funcion que esta en ensamblador

// BANDERAS (evitar si posible)

volatile uint8_t parpadearFlag = 0;  // si habilitado, se parpadea el LED
volatile uint8_t ultimoFlag = 0; // indica cuando ha pasado el timer de 30 segundos (1-si, 0-no)
volatile uint8_t habilitarEstadoFinal = 1;  // si habilitado pasa al estado final despues de lanzar
volatile uint8_t timerTicks = 0;  // ticks para medir los 30 segundos
uint8_t oscillando = 0;  // flag que se habilita cuando esta oscillando el lanzador

/************************************************************************/
// Interrupciones:

ISR(TIMER0_COMPA_vect){						//Interrupcion que ocurre cada 5ms
	DisplayUpdater();  // update displays with high frecuency 
	updateTime();		// update Lanzadores Timer
	if(parpadearFlag==1){
		parpadearLED();   // enable parpadeo if necessary
	}
}

ISR(TIMER1_COMPA_vect){  // cuando pasan los 30 segundos (10*3 segundos del timer 1)
	timerTicks++;
	if(timerTicks==10){
		ultimoFlag = 1;
		disableTimer1Int();
		timerTicks = 0;
	}
}

ISR(PCINT2_vect){ //Interrupcion PCINT de los sensores opticos
	OnPinChangeBolos();
}

// interrupcon del SW2 (lanzador)
// solo me interesan durante el estado de LANZAMIENTO
ISR(INT0_vect){
	if(state==LANZAMIENTO){
		OnSW2Interruption();
	}
}

ISR(INT2_vect){  // interrupcion del pulsador del disparo (SW6), maneja los cambios de estado cuando se pulsa
	if(state == LANZAMIENTO){
		habilitarInterrupcionesSensores();
		state = TIRAR_BOLA;
	}
	if((state == HOME) && (puntuacion==0)){
		state = SIN_BOLA;
	}
	if(state==FINAL){
		state = HOME;
	}
}

/************************************************************************/
// Explicacion de los estados:
// HOME: el setup esta hecho y se puede empezar el juego pulsando SW6
// SIN_BOLA: la bola esta en el elevador de cargas, pero no en el lanzador
// BOLA_LANZADOR: la bola esta colocada en el lanzador
// LANZAMIENTO: el sistema se encuentra el el proceso de lanzar la bola
// TIRAR_BOLA: la bola fue lanzada para tirar los bolos
// FINAL: el juego esta finalizado y el display parpadea

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
	homeEC();
	vastagoHome();
	carritoHome();
	lanzadorHome();
    while (1) {
		switch(state){
			case HOME:  // primero estado despues del setup
				ultimoFlag = 0;
				parpadearFlag = 0;
				finalizadoFlag = 0;
				puntuacion = 0;
				lanzadorHome();
				// la bandera habilitar estado final se resetea despues
				// no hacemos nada, esperamos la interrupcion de disparo
				encenderLED();
				break;
			case SIN_BOLA:
				apagarLED();
				subeEC();  // comprobar que EC esta en posicion alta
				girarLanzador(1);
				moverVastagoAtras();  // mover vastago atras para evitar que choca (ya esta)
				_delay_ms(long_delay);
				bajaEC();  // coger la bola
				_delay_ms(doublePressbuffer);
				subeEC(); // subir la bola		
				bajaER();
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
				
				if(oscillando==0){
					girarLanzador(0);  // girar hacia la izquierda
					_delay_ms(long_delay);  // hasta que llega a la iqzuierda
					oscillando = 1;
				}
				
				state = LANZAMIENTO; // empezar lanzamiento
				
				break;

			case LANZAMIENTO:
				
				bajaER();
				if(ultimoFlag){
					parpadearFlag = 1;
				}
				else{
					encenderLED();
				}
				
				// esperar interupciones
				// loop_until_bit_is_set(SW6PIN,SW6X);  //esperar hasta se pulsa el disparo
				// cuando se interumpe, marcar state=TIRAR_BOLA
				// state = TIRAR_BOLA;
				break;
					
			case TIRAR_BOLA:
			
				if(habilitarEstadoFinal==1 && ultimoFlag==0){  // empezar timer de 30 segundos
					resetTimer1();
					enableTimer1Int();
					puntuacion = 0;
					timerTicks = 0;
					habilitarEstadoFinal = 0;  // resetear flag de estado final
				}
				
				oscillando=0;  // deshabilitar bandera de oscillacion
				
				// El interruptor de disparo se ha pulsado
				parpadearFlag = 0;  // apagar led despues del disparo (y parpadeo)
				apagarLED();
				
				if(ultimoFlag==1){  // si ya se hizo la ultima tirada
					habilitarEstadoFinal = 1;  // pasar al estado final despues del retorno
				}
				
				frenoLanzador();  // frenamos el lanzador
				_delay_ms(button_check_delay_ms);
				liberarCarrito();  // tiramos la bola
				_delay_ms(button_check_delay_ms);
				loop_until_bit_is_clear(SW4PIN,SW4X);
				pararCarrito();

				state = RETORNO;  // inicar estado de retorno
				break;
			case RETORNO:
				_delay_ms(ballArrivalDelay);   // esperar hasta la bola llega

				subeER();
				deshabilitarInterrupcionesSensores();  // evitar otras interrupciones de los bolos
				_delay_ms(3000);
				if(habilitarEstadoFinal==1){ // si hemos tirado la bola y ya estamos en el ultimo disparo
					state = FINAL;  // ir al estado de parpadeo
				}
				else{
					state = SIN_BOLA; // volver a recoger la bola 
				}
				break;
			case FINAL:
				finalizadoFlag = 1;  // parpadear display
				_delay_ms(2000);
				bajaER();
				break;
		}
    }
}

