#ifndef IncFile1_h__
#define IncFile1_h__

#ifndef F_CPU
#define F_CPU 8000000UL   // comprobar que la frecuencia es 8MHz y no 16MHz
#endif

/************************************************************************/
/*  Instrucciones:                                                                     
*	Los Macros PORT, DDR, PIN guardan el registro correspondiente
*	Los Macros SWnX tienen el numero del pin
*	Esto se puede usar con setInput(SW1PORT,SW1X) = setInput(PORTD,PD6)
*/
/************************************************************************/
// CONSTANTES
#define button_check_delay_ms 200
#define debounce_buffer_ms 50
#define long_delay 1000
#define doublePressbuffer 500

/************************************************************************/
//MACROS SW

	// deprecated
	//#define L2DDR(L)	DD##L
	//#define L2PIN(L)	PIN##L
	//#define L2PORT(L)	PORT##L

	#define SW1PORT PORTD
	#define SW1DDR	DDRD
	#define SW1PIN	PIND
	#define SW1X	PD6
	
	#define SW2PORT PORTD
	#define SW2DDR	DDRD
	#define SW2PIN	PIND
	#define SW2X	PD0
	
	#define SW3PORT PORTD
	#define SW3DDR	DDRD
	#define SW3PIN	PIND
	#define SW3X	PD1
	
	#define SW4PORT PORTD
	#define SW4DDR	DDRD
	#define SW4PIN	PIND
	#define SW4X	PD4   // cambio!
	
	#define SW5PORT PORTD
	#define SW5DDR	DDRD
	#define SW5PIN	PIND
	#define SW5X	PD3
	
	#define SW6PORT PORTD
	#define SW6DDR	DDRD
	#define SW6PIN	PIND
	#define SW6X	PD2  //cambio!
	
	#define SWEICR 	EICRA //configurar en función de las necesidades
	#define SWEIMSK EIMSK //0b00001111
	
	#define SW2EIFR INTF0
	#define SW3EIFR INTF1
	#define SW6EIFR INTF2  // cambio
	#define SW5EIFR INTF3

/************************************************************************/
//MACROS Motores

	#define M1_diPORT	PORTK
	#define M1_diDDR	DDRK
	#define M1_diPIN	PINK
	#define M1_di_X		PK6

	#define M1_enPORT	PORTL
	#define M1_enDDR	DDRL
	#define M1_enPIN	PINL
	#define M1_en_X		PL0

	#define M2_diPORT	PORTK
	#define M2_diDDR	DDRK
	#define M2_diPIN	PINK
	#define M2_di_X		PK7

	#define M2_enPORT	PORTL
	#define M2_enDDR	DDRL
	#define M2_enPIN	PINL
	#define M2_en_X		PL1

	#define M2_bkPORT	PORTL
	#define M2_bkDDR	DDRL
	#define M2_bkPIN	PINL
	#define M2_bk_X		PL3

	#define M3_diPORT	PORTL
	#define M3_diDDR	DDRL
	#define M3_diPIN	PINL
	#define M3_di_X		PL2

	#define M3_enPORT	PORTL
	#define M3_enDDR	DDRL
	#define M3_enPIN	PINL
	#define M3_en_X		PL4

	#define M4_diPORT	PORTL
	#define M4_diDDR	DDRL
	#define M4_diPIN	PINL
	#define M4_di_X		PL5

	#define M4_enPORT	PORTL
	#define M4_enDDR	DDRL
	#define M4_enPIN	PINL
	#define M4_en_X		PL7

	#define M5_diPORT	PORTD
	#define M5_diDDR	DDRD
	#define M5_diPIN	PIND
	#define M5_di_X		PD5

	#define M5_enPORT	PORTD
	#define M5_enDDR	DDRD
	#define M5_enPIN	PIND
	#define M5_en_X		PD7

/************************************************************************/
// MACROS Sensores Opticos

	#define SOPCICR		PCICR //0b00000100
	#define SOPCMSK		PCMSK2//0b00111111
	#define SOPCIFR		PCIFR
	
	#define SO1PIN		PINK0
	#define SO2PIN		PINK1
	#define SO3PIN		PINK2
	#define SO4PIN		PINK3
	#define SO5PIN		PINK4
	#define SO6PIN		PINK5
	#define SOPIN		PINK
	

/************************************************************************/
//MACROS LEDs y Display

	#define L1PORT		PORTL
	#define L1DDR		DDRL
	#define L1PIN		PINL
	#define L1X			PL6

	#define DaPORT		PORTB
	#define DaDDR		DDRB
	#define DaPIN		PINB
	#define DaX			PB0
	
	#define DbPORT		PORTB
	#define DbDDR		DDRB
	#define DbPIN		PINB
	#define DbX			PB1
	
	#define DcPORT		PORTB
	#define DcDDR		DDRB
	#define DcPIN		PINB
	#define DcX			PB2
	
	#define DdPORT		PORTB
	#define DdDDR		DDRB
	#define DdPIN		PINB
	#define DdX			PB3
	
	#define DePORT		PORTB
	#define DeDDR		DDRB
	#define DePIN		PINB
	#define DeX			PB4

	#define DfPORT		PORTB
	#define DfDDR		DDRB
	#define DfPIN		PINB
	#define DfX			PB5
	
	#define DgPORT		PORTB
	#define DgDDR		DDRB
	#define DgPIN		PINB
	#define DgX			PB6
	
	#define DSPORT		PORTB
	#define DSDDR		DDRB
	#define DSPIN		PINB
	#define DSX			PB7

/************************************************************************/
//MACROS:
// B bit; P registro (p.e. D), V valor, interrupcion I
#define setBit(P, B) (P |= (1 << B))

#define clearBit(P, B) ((P) &= ~(1 << B))

//invertir bit
#define toggleBit(P, B) ((P)^=(1 << B))

// da el valor del bit
// #define readBit(P, B) ((P >> B) & 0x01)
#define readBit(P, B) (((P) & (1 << B)) >> B)

#define setOutput(P, B) ((P) |= (1 << B))
#define setInput(P, B) ((P) &= ~(1 << B))

#define writePWM(P, V) (OCR##P## = V)

// habilitar interrupciones externas
#define enableInterrupt(I) (EIMSK |= (1 << I))
#define disableInterrupt(I) (EIMSK &= ~(1 << I))
 
// para if/else, comprueba si pin numero B de puerto P es high/low
#define isHigh(P, B) (PIN##P & (1 << B))
#define isLow(P, B) (!(PIN##P & (1 << B)))

// funciones equivalentes a set/clear bit, pero tambien usando mascaras para cambiar multiples pines
#define setBitMask(port, mask) ((port) |= (mask))  // pone un 1 a todos los valores 1 del mask
#define clearBitMask(port, mask) ((port) &= ~(mask)) // pone un 0 a todos los valores 1 del mask

#endif // IncFile1_h__