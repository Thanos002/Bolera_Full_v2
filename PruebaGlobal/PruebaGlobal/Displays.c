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

volatile uint16_t parpadeos=0;  // volatile pues se cambia con INT
volatile uint8_t finalizadoFlag = 0;
uint16_t pulsador;
volatile uint8_t puntuacion;

inline void apagarDisplay(){
	clearBitMask(DSPORT, 0b01111111);
	//PORTB &= ~(1<<PB6)&~(1<<PB5)&~(1<<PB4)&~(1<<PB3)&~(1<<PB2)&~(1<<PB1)&~(1<<PB0);  //Todos los bits de esos puertos a 0
}

inline void setDisplay(int numero){
	//Primero hago un lienzo, en el que pongo los displays apagados durante el ciclo de reloj para después poder poner el numero al gusto
	apagarDisplay();
	//PORTB = 0bX0000000
	switch(numero)
	{
		case 0:	//0b00111111;
		DSPORT |= (1<<DaX)|(1<<DbX)|(1<<DcX)|(1<<DdX)|(1<<DeX)|(1<<DfX);
		break;
		case 1:	//0b00000110;
		DSPORT |= (1<<DbX)|(1<<DcX);
		break;
		case 2: //0b01011011;
		DSPORT |= (1<<DaX)|(1<<DbX)|(1<<DdX)|(1<<DeX)|(1<<DgX);
		break;
		case 3: //0b01001111;
		DSPORT |= (1<<DgX)|(1<<DdX)|(1<<DcX)|(1<<DbX)|(1<<DaX);
		break;
		case 4: //0b01100110;
		DSPORT |= (1<<DbX)|(1<<DcX)|(1<<DfX)|(1<<DgX);
		break;
		case 5: //0b01101101;
		DSPORT |= (1<<DaX)|(1<<DcX)|(1<<DdX)|(1<<DfX)|(1<<DgX);
		break;
		case 6: //0b01111101;
		DSPORT |= (1<<DaX)|(1<<DfX)|(1<<DcX)|(1<<DdX)|(1<<DeX)|(1<<DgX);
		break;
		case 7: //0b00000111;
		DSPORT |= (1<<DaX)|(1<<DbX)|(1<<DcX);
		break;
		case 8: //0b01111111;
		DSPORT |= (1<<DaX)|(1<<DbX)|(1<<DcX)|(1<<DdX)|(1<<DeX)|(1<<DfX)|(1<<DgX);
		break;
		case 9: //0b01101111;
		DSPORT |= (1<<DaX)|(1<<DbX)|(1<<DcX)|(1<<DfX)|(1<<DgX);
		break;
		default:
		DSPORT |= (1<<DgX);
	}
}

inline void DisplayUpdater(){		//Se llama cada 5ms con el timer0
	if(finalizadoFlag == 1){
		parpadeos++;
		if(parpadeos <= 180){
			
			toggleBit(DSPORT,DSX);				//Máscara en el bit de seleccion para alternarlo cada 5 ms, es decir un refresco de 200 veces/segundo
			//Posible bandera 'Selector
			
			if(~PINB & (1 << DSX)){				//Si está seleccionado Display 1, decenas
				setDisplay(puntuacion/10);
			}
			else{								//Si esta seleccionado Display 0, unidades
				setDisplay(puntuacion%10);
			}
		}
		else if(parpadeos >= 180){
			DSPORT = 0x00;  // Apagados y DS da igual porque al estar apagados da igual a que display este apuntando
			if(parpadeos == 200){
				parpadeos = 0; // Reiniciar el contador de parpadeos
			}
		}
	}
	else{
		//Preguntar Jorge pero creo que PORT7B no vale, tiene que ser PORTXi --> PXi para los puertos de E/S. No lo que pone en el datasheet, para este caso.
		toggleBit(DSPORT,DSX);					//Máscara en el bit de seleccion para alternarlo cada 5 ms, es decir un refresco de 200 veces/segundo
		//Posible bandera 'Selector
		
		if(~PINB & (1 << DSX)){				//Si está seleccionado Display 1, decenas
			setDisplay(puntuacion/10);
		}
		else{								//Si esta seleccionado Display 0, unidades
			setDisplay(puntuacion%10);
		}
	}
}






