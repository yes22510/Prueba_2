//*******************
//Universidad del Valle de Guatemala
//IE 2023: Programación de Microcontroladores
//Autor: Ever Yes
//Proyecto: Contador 4 bits
//Descripción: Codigo de ejemplo contador de 4 bits
// Hardware: ATMega328P
//Created: 26-01-2024
//******************
//Encabezado
//*********************
 
.include "M328PDEF.inc"
.cseg 
.org 0x00

//********************
//Configuración de la Pila
//********************
LDI R16,LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17
//*********************


//Configuracion de E/S
SETUP:
	LDI R16, 0b0010_0000
	LDI R16, (1<< CLKPCE)
	STS CLKPR, R16			; Habilita el prescaler

	LDI R16, 0B0000_0001
	STS CLKPR, R16			; Se define un prescaler de 8fcpu = 2MHz 
	LDI r16, 0xFF
	OUT DDRD, r16//salida
	OUT DDRB, r16//salida
	OUT PORTC, r16			;Configurar entradas con pullup
	CLR r16
	OUT DDRC, r16// Entrada
read_sw:
	CLR r23
	MOV r23, r26
	MOV r24, r22
	MOV r25, r16
	swap r22
	STS PORTD, r22
	STS PORTB, r6
	IN r20, PINC

	//reset
	ANDI r20, 0X1F
	BREQ reset

	//Primer boton Ascendente
	CPI r20, 0x1E
	BREQ ascendente

	//Primer boton Descendente
	CPI r20, 0x1D
	BREQ descendente

	//Segundo boton Ascendente
	CPI r20, 0x1B
	BREQ ascendente2

	//Segundo boton Descendente
	CPI r20, 0x17
	BREQ descendente2
	
	//Sumador
	CPI r20, 0x0F
	BREQ Suma

	//stop
	CPI r20, 0x1f
	BREQ stop

reset:
	CLR r16
	RJMP read_sw

ascendente:
	cpi r16, 0x0F
    BREQ read_sw
	INC r16
	RCALL delay
	RJMP read_sw

descendente:
	cpi r16, 0x00
    BREQ read_sw
	DEC r16
	RCALL delay
	RJMP read_sw

ascendente2:
	cpi r22, 0x0F
    BREQ read_sw
	INC r22
	RCALL delay
	RJMP read_sw

descendente2:
	cpi r22, 0x00
    BREQ read_sw
	DEC r22
	RCALL delay
	RJMP read_sw
Suma:
	ADD r24,r25
	MOV r26,r24
	Rcall delay
	rjmp read_sw

stop:
	RJMP read_sw

delay:
	LDI r17, 90

ANT0:
	LDI r18, 90


ANT1:
	LDI r19, 90

ANT2:
	NOP
	DEC r19
	BRNE ANT2
	DEC r18
	BRNE ANT1
	DEC r17
	BRNE ANT0
	ret     