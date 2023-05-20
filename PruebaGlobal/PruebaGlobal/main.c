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

// BANDERAS (evitar si posible)

uint8_t parpadear =0;
uint8_t buffer =0;

// Interrupciones:

ISR(TIMER0_COMPA_vect){						//Interrupcion que ocurre cada 5ms
	DisplayUpdater();
	updateTime();
	UpdateTimeElevadores();
	if(parpadear){
		parpadearLED();
	}
}

ISR(PCINT2_vect){ //Cuidado que era por flanco de bajada
	OnPinChangeBolos();
}

// interrupcon del SW2 que sirva para distinguir que pulsador se ha pulsado
ISR(INT0_vect){
	OnSW2Interruption();
}

ISR(INT2_vect){
	if(state == LANZAMIENTO){
		state = TIRAR_BOLA;
	}
	if(state == HOME){
		state = SIN_BOLA;
	}
}

ISR(INT3_vect) {
	OnSW5Interruption();
}

int main(void)
{
	setup_puertos();
	setupLanzador();
	setup_timer0();
	setup_timer1();
	setupElevadores();
	homeER();
	bajaER();
	homeEC();
	subeEC();
	lanzadorHome();  // comprobar si se puede hacer el home de manera segura (ELEVADOR)
    while (1) {
		switch(state){
			case HOME:
				loop_until_bit_is_set(SW6PIN,SW6X);
				state = SIN_BOLA;
			break;
			case SIN_BOLA:
				subeEC();  // comprobar que EC esta en posicion alta
				_delay_ms(1000);
				girarVertical(1);
				_delay_ms(2000);
				bajaEC();  // coger la bola
				_delay_ms(2000);
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
				buffer = getTime();  // timer de 30 segundos
			break;

			case LANZAMIENTO:
					
				// encender LED
				encenderLED();
					
				// despues de 30 seg. se habilita la bandera parpadear
				// Integracion: posiblemente sacarlo de los estados al nivel del main, asi se acualiza independientemente del estado
				if(buffer+30000<getTime()){
					parpadear = 1;
				}
				girarVertical(0);
				girarVertical(1);
				// esperar interupciones
				// alternativa: atender mediante interrupcion de SW6 (PCINT2)
				loop_until_bit_is_set(SW6PIN,SW6X);  //esperar hasta se pulsa el disparo
				// cuando se interumpe, marcar state=TIRAR_BOLA
				state = TIRAR_BOLA;

			break;
					
			case TIRAR_BOLA:
				// asegurar que estamos a la izquierda del Lanzador:
				if(position==RIGHT){
					state = LANZAMIENTO;  // si no, procedemos con el lanzamiento
					girarVertical(0);  // opcional: intentar corregir posicion
					break;
				}
					
				// El interruptor de disparo se ha pulsado
				apagarLED();
				frenoVertical();
				_delay_ms(button_check_delay_ms);
				liberarCarrito();
				_delay_ms(long_delay);
				pararCarrito();

				state = RETORNO;
			case RETORNO:
				_delay_ms(6000);
				subeEC();
				_delay_ms(5000);
				bajaEC();
				// si hemos tirado la bola y ya estamos en el ultimo disparo
				if(parpadear){
					state = HOME;  // volver a primer estado de espera
				}
				else{
					state = SIN_BOLA;
				}
			break;
				
		}
    }
}

