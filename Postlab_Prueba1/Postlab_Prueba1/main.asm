//*******************
//Universidad del Valle de Guatemala
//IE 2023: Programación de Microcontroladores
//Autor: Ever Yes
//Proyecto: Prelab2
//Descripción: Contador 4 bits
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
//***********************************
//TABLA DE VALORES
//***********************************
			TABLA7SEG: .DB 0B01000000, 0B01111001, 0b00100100, 0b00110000, 0b00011001, 0b0010010, 0b0000010, 0b01111000, 0b00000000, 0b00010000, 0b00001000, 0b00000011, 0b001000110, 0b00100001, 0b00000110, 0b00001110
							//0            1           2         3             4          5           6          7           8         9             10           11        12           13           14          15
							
//***********************************
SETUP:
	LDI R16, 0b0010_0000
	LDI R16, (1<< CLKPCE)
	STS CLKPR, R16			; Habilita el prescaler

	LDI R16, 0B0000_0001
	STS CLKPR, R16			; Se define un prescaler de 8fcpu = 2MHz 
	
	LDI r16, 0XFF
	OUT DDRC, R16
	OUT DDRB, R16
	ldi R16, 0b00110000
	OUT DDRD, R16
	LDI R20, 0
	LDI ZH, HIGH(TABLA7SEG<<1)
	LDI ZL, LOW(TABLA7SEG<<1)
Timer0:
	LDI R16, (1<<CS02) |  (1<<CS00)
	OUT TCCR0B, R16
	
	LDI R16, 135		;CARGAR EL VALOR DE DESBORDAMIENTO
	OUT TCNT0, R16			;Cargar el valor incial 
	RJMP VerificacionB

VerificacionB:
	IN r24, PIND
	ANDI R24, 0b00001100
	CPI r24, 4
	breq Incremento7seg

	CPI r24, 8
	breq Decremento7seg

	CPI r24, 0
	BREQ EncenLuc


EncenLuc:
	LDI ZH, HIGH(TABLA7SEG<<1)
	LDI ZL, LOW(TABLA7SEG<<1)
	ADD	ZL, R23
	ldi r18, 0
	LDI r19, 0

	LPM R18, Z
	MOV R19, R18
	ldi r21, 0x1F
	AND r18, r21
	ldi r21, 0b01100000
	AND r19, r21
	lsr r19
	OUT PORTC, R18
	OUT PORTD, r19
	IN R16, TIFR0
	CPI R16, (1<<TOV0)
	BRNE VerificacionB		;Si no esta seteada, continuar esperando 

	LDI R16, 195
	OUT TCNT0, R16
	;STS UCSR0B, R19		Usar PD0 PD1
	SBI TIFR0, TOV0
	INC R25
	CPI R25, 10
	BRNE VerificacionB
ContT:
	MOV r19, r22
	ANDI r22, 0b00001111
	CPI r23, 0
	BREQ Salto3
	ANDI r19, 0b00100000
	CPI r19, 0b00100000
	BRNE LedO
	CP r22, r23
	BRNE Salto5
	LDI r22, 0b00000000
	RJMP Vres
Ledo:
	CP r22, r23
	BRNE Salto3
	ldi r22,0b00100000
	RJMP Vres
Salto5:
	LDI r19, 0b00100000
	or r22, r19
Salto3:
	INC r22


Vres:
	OUT PORTB, r22
	RJMP VerificacionB
Incremento7seg:
	; Antes de realizar cambios en el 7 segmentos
	ldi R16, (0 << CS02) | (0 << CS01) | (0 << CS00)  ; Desactivar temporizador
	out TCCR0B, R16
	cpi r20, 0x0F
    BREQ Salto
	INC r20
Salto:
	rcall delay
	MOV R23, R20
	;ADD	ZL, R23
	RJMP Timer0
//*************************************
Decremento7seg:
	; Antes de realizar cambios en el 7 segmentos
	ldi R16, (0 << CS02) | (0 << CS01) | (0 << CS00)  ; Desactivar temporizador
	out TCCR0B, R16
	cpi r20, 0x00
    BREQ VerificacionB
	DEC r20
	rcall delay
	MOV R23, R20
	ADD	ZL, R23
	RJMP Timer0
stop:
	RJMP VerificacionB

delay:
	LDI r17, 100

ANT0:
	LDI r18, 100


ANT1:
	LDI r19, 100

ANT2:
	NOP
	DEC r19
	BRNE ANT2
	DEC r18
	BRNE ANT1
	DEC r17
	BRNE ANT0
	ret   
Reset:
	ldi r22, 0
	RJMP Vres
