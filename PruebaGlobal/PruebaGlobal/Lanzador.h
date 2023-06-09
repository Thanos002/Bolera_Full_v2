/*
* Lanzador.h
*/

#ifndef LANZADOR_H_
#define LANZADOR_H_

#include "IncFile1.h"
#include "Bolera.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Function prototypes
void setupLanzador();
void lanzadorHome();
void vastagoHome();
void carritoHome();
void rightInterrupt();
void middleInterrupt();
void leftInterrupt();
uint8_t getParpadeo();

void updateTime();
void setTime(int time);
uint32_t getTime();

typedef enum {HOME, SIN_BOLA, BOLA_LANZADOR, LANZAMIENTO, TIRAR_BOLA, RETORNO, FINAL} States;
void OnSW2Interruption();
extern volatile States state;

typedef enum {LEFT, RIGHT} Position;
extern volatile Position position;
extern volatile uint8_t parpadearFlag;

#endif /* LANZADOR_H_ */
