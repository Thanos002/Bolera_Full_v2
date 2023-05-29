.globl setup_timer0  // definir setup_timer0 como funcion global
.type setup_timer0, @function  // 
setup_timer0:
	push r24      // Push the value of r24 onto the stack
	cli
	ldi r24, 0b00000010 // Load the value of OCR0A into r24
	out 0x24, r24
	ldi r24, 0b00000100
	out 0x25, r24
	ldi r24, 0b00000010
	//out 0x6e, r24
	//sbi 0x6e, 2
	sts 0x6e, r24
	ldi r24, 0x9C
	out 0x27, r24
	sei
	pop r24    // Restore the original value of r24 from the stack