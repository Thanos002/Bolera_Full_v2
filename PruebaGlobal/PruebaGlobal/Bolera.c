
#include <avr/io.h>
#include "Bolera.h"
#include "IncFile1.h"
#include "ConfiguracionV1.h"

// dir -> 1 derecha, 0 izquierda

// Los define MACROS estan en IncFile1.h

// *******
// MOTORES
// *******

// M1: Elevador de Cargas
inline void subirElevadorCargas(){
	pararElevadorCargas();
	setBit(M1_diPORT, M1_di_X);
	setBit(M1_enPORT, M1_en_X);
}

inline void bajarElevadorCargas(){
	pararElevadorCargas();
	clearBit(M1_diPORT, M1_di_X);
	setBit(M1_enPORT, M1_en_X);
}

inline void pararElevadorCargas(){
	clearBit(M1_enPORT, M1_en_X);
}


// M2: Giro vertical del lanzador

// default: lanzadorflag a 2 (=parado)
uint8_t lanzadorFlag = 2;

inline void girarLanzador(int dir){
	clearBit(M2_enPORT, M2_en_X);  // parar motor
	lanzadorFlag = dir;
	if (dir==1){ 
		setBit(M2_diPORT, M2_di_X);
	}
	else if (dir==0){
		clearBit(M2_diPORT, M2_di_X);
	}
	setBit(M2_enPORT, M2_en_X);  // enable motor
	clearBit(M2_bkPORT,M2_bk_X);  // deshabilitar freno
}

inline void frenoLanzador(){  // frenado instataneo
	setBit(M2_bkPORT,M2_bk_X);
	clearBit(M2_enPORT,M2_en_X);
	lanzadorFlag = 2;
}

inline void pararLanzador(){  // se utiliza para deshabilitar el motor
	clearBit(M2_enPORT, M2_en_X);
	lanzadorFlag = 2;
}

// M3: Vastago

inline void moverVastagoAdelante(){
	pararVastago();
	setBit(M3_diPORT, M3_di_X);
	setBit(M3_enPORT, M3_en_X);
}
inline void moverVastagoAtras(){
	pararVastago();
	clearBit(M3_diPORT, M3_di_X);
	setBit(M3_enPORT, M3_en_X);
}
inline void pararVastago(){
	clearBit(M3_enPORT, M3_en_X);
}

// M4: Carrito

inline void engancharCarrito(){
	pararCarrito();
	setBit(M4_diPORT, M4_di_X);
	setBit(M4_enPORT, M4_en_X);
}
inline void liberarCarrito(){
	pararCarrito();
	clearBit(M4_diPORT, M4_di_X);
	setBit(M4_enPORT, M4_en_X);
}
inline void pararCarrito(){
	clearBit(M4_enPORT, M4_en_X);
}

// M5: Elevador de retorno de bolas

inline void subirElevadorRetorno(){
	pararElevadorRetorno();
	setBit(M5_diPORT, M5_di_X);
	setBit(M5_enPORT, M5_en_X);
}

inline void bajarElevadorRetorno(){
	pararElevadorRetorno();
	clearBit(M5_diPORT, M5_di_X);
	setBit(M5_enPORT, M5_en_X);
}

inline void pararElevadorRetorno(){
	clearBit(M5_enPORT, M5_en_X);
}

// *******
// SENSORES SW
// *******

// getters para los sensores (leer pins directamente)
// Sensor SW1:

inline int getSensor1(){
	return readBit(SW1PIN,SW1X);
}

// Sensor SW2:
// ojo: aqui fin de carrera tambien en la mitad del trayecto!

inline int getSensor2(){
	return readBit(SW2PIN,SW2X);
}

// Sensor SW3:

inline int getSensor3(){
	return readBit(SW3PIN,SW3X);
}

// Sensor SW4:

inline int getSensor4(){
	return readBit(SW4PIN,SW4X);
}

// Sensor SW5:

inline int getSensor5(){
	return readBit(SW5PIN,SW5X);
}

// Sensor SW6:

inline int getSensor6(){
	return readBit(SW6PIN,SW6X);
}

// *******
// SENSORES OPTICOS
// *******

//MACROS

// getter para los sensores opticos
inline int getOptico(int num){
	return readBit(SOPIN, num);
}

// *******
// LED Y DISPLAYS
// *******

inline void encenderLED(){
	setBit(L1PORT, L1X);
}

inline void apagarLED(){
	clearBit(L1PORT, L1X);
}

