/*
* Displays.c
*
* Created: 15/05/2023 17:28:39
* Author : mrjav
*/

#include "Displays.h"
#include "ConfiguracionV1.h"
#include "IncFile1.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

uint16_t parpadeos=0;
uint8_t estado_final = 0;
uint16_t pulsador;
uint8_t puntuacion;

/*Ya hecho en ConfiguracionV1
// void setup_Displays(){
// 	cli();
//     setup_timer0();			//Para el setup del timer 0 Displays
// 	DDRB = 0xFF;				//Puerto B configurado como salidas para controlar display 7 segmentos y selector Displays
//
// 	//El pin 7 del puerto B controla la selección de displays, los otros 7 pines el numero que aparece un '0' en la señal enciende el display
//	PORTB = 0b00111111;				//Inicialemente a 0 en el display 0
//
// 	DDRD = 0b00100000;				//Entradas salvo el bit 5 que es salida para motor (SW1-SW6) Displays
// 	//Ver si esos sensores SW son normalemente activos o bajos...
//
//
// 	sei();
//
*/

void setDisplay(int numero){
	
	//Primero hago un lienzo, en el que pongo los displays apagados durante el ciclo de reloj para después poder poner el numero al gusto
	PORTB &= ~(1<<PB6)&~(1<<PB5)&~(1<<PB4)&~(1<<PB3)&~(1<<PB2)&~(1<<PB1)&~(1<<PB0);  //Todos los bits de esos puertos a 1_
	//PORTB = 0bX0000000
	switch(numero)
	{
		case 0:	//0b00111111;
		PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5);
		break;
		case 1:	//0b00000110;
		PORTB |= (1<<PB1)|(1<<PB2);
		break;
		case 2: //0b01011011;
		PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB3)|(1<<PB4)|(1<<PB6);
		break;
		case 3: //0b01001111;
		PORTB |= (1<<PB6)|(1<<PB3)|(1<<PB2)|(1<<PB1)|(1<<PB0);
		break;
		case 4: //0b01100110;
		PORTB |= (1<<PB1)|(1<<PB2)|(1<<PB5)|(1<<PB6);
		break;
		case 5: //0b01101101;
		PORTB |= (1<<PB0)|(1<<PB2)|(1<<PB3)|(1<<PB5)|(1<<PB6);
		break;
		case 6: //0b01111101;
		PORTB |= (1<<PB0)|(1<<PB5)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB6);
		break;
		case 7: //0b00000111;
		PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
		break;
		case 8: //0b01111111;
		PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5)|(1<<PB6);
		break;
		case 9: //0b01101111;
		PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB5)|(1<<PB6);
		break;
		default:
		PORTB |= (1<<PB6);
	}
}

inline void DisplayUpdater(){
	if(estado_final == 1){
		parpadeos++;
		if(parpadeos <= 180){
			//Preguntar Jorge pero creo que PORT7B no vale, tiene que ser PORTXi --> PXi para los puertos de E/S. No lo que pone en el datasheet, para este caso.
			PORTB ^= (1<<PB7);					//Máscara en el bit de seleccion para alternarlo cada 5 ms, es decir un refresco de 200 veces/segundo
			//Posible bandera 'Selector
			
			if(PINB & (1 << PB7)){				//Si está seleccionado Display 1, decenas
				setDisplay(puntuacion/10);
			}
			else{								//Si esta seleccionado Display 0, unidades
				setDisplay(puntuacion%10);
			}
		}
		if(parpadeos >= 180){
			PORTB = 0x00;  // Apagados y DS da igual porque al estar apagados da igual a que display este apuntando
		}
		if(parpadeos == 200){
			parpadeos = 0; // Reiniciar el contador de parpadeos
		}
	}
	else{
		//Preguntar Jorge pero creo que PORT7B no vale, tiene que ser PORTXi --> PXi para los puertos de E/S. No lo que pone en el datasheet, para este caso.
		PORTB ^= (1<<PB7);					//Máscara en el bit de seleccion para alternarlo cada 5 ms, es decir un refresco de 200 veces/segundo
		//Posible bandera 'Selector
		
		if(PINB & (1 << PB7)){				//Si está seleccionado Display 1, decenas
			setDisplay(puntuacion/10);
		}
		else{								//Si esta seleccionado Display 0, unidades
			setDisplay(puntuacion%10);
		}
	}
}






