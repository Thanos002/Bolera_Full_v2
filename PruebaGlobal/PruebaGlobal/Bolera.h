/*
 * Bolera.h
 * Inlcude file that provides basic functionality, such as motor control and sensor getters
 */

#ifndef BOLERA_H
#define BOLERA_H

#include <avr/io.h>
#include "IncFile1.h"

// dir -> 1 derecha, 0 izquierda

// Los define MACROS estan en IncFile1.h

// *******
// MOTORES
// *******

// M1: Elevador de Cargas
void subirElevadorCargas();
void bajarElevadorCargas();
void pararElevadorCargas();

// M2: Giro vertical del lanzador
extern uint8_t lanzadorFlag;
void girarVertical(int dir);
void frenoVertical();
void pararVertical();

// M3: Vastago
void moverVastagoAdelante();
void moverVastagoAtras();
void pararVastago();

// M4: Carrito
void engancharCarrito();
void liberarCarrito();
void pararCarrito();

// M5: Elevador de retorno de bolas
void elevarBola();
void bajarBola();

// *******
// SENSORES SW
// *******

// Sensor SW1:
int getSensor1();

// Sensor SW2:
// ojo: aqui fin de carrera tambien en la mitad del trayecto!
int getSensor2();

// Sensor SW3:
int getSensor3();

// Sensor SW4:
int getSensor4();

// Sensor SW5:
int getSensor5();

// Sensor SW6:
int getSensor6();

// *******
// SENSORES OPTICOS
// *******

// MACROS
int getOptico(int num);

// *******
// LED Y DISPLAYS
// *******

void encenderLED();
void apagarLED();
void parpadearLED();

#endif // BOLERA_H
