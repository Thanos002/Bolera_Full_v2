.globl setup_timer0  // definir setup_timer0 como funcion global
.type setup_timer0, @function  // desclarar setup_timer0 como funcion
setup_timer0:
	push r24      // poner el valor de r24 en la pila
	cli
	ldi r24, 0b00000010 //TCCR0A = 0b00000010; Modo CTC
	out 0x24, r24
	ldi r24, 0b00000100 //TCCR0B = 0b00000100; Prescalado 256
	out 0x25, r24
	ldi r24, 0b00000010  //TIMSK0 = 0b00000010;	Habilita OCR0A 
	sts 0x6e, r24  // sts porque el registro es memory mapped
	ldi r24, 0x9C  // OCR0A =  156; para contar cada 5ms
	out 0x27, r24
	sei
	pop r24    // restaurar el valor de r24 de la pila 