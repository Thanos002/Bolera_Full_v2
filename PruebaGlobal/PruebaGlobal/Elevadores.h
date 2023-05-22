#include <avr/io.h>

#ifndef ELEVADORES_H_
#define	ELEVADORES_H_

//Prototipo de funciones

void setuptimer();

void setupEC(); 

void setupER();

void bajaEC();

void subeEC();

void homeEC();
int waitforEC(int estado);//La variable estado correspode a la posición en la que debe acabar el elevador . El programa esperará hasta que llegue a ella;

void homeER();
void bajaER();

void subeER();

int getEstadoER();//Devuelve la posición del ER

void UpdateTimeElevadores();
void setupPulsador();
void setupElevadores();
void OnSW5Interruption();

void recarga();//Le pasamos el tiempo de delay y la condición del ultimo disparo y procede a recargar la bola
#endif