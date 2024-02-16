//***
//Universidad del Valle de Guatemala
//IE 2023: Programación de Microcontroladores
//Autor: Ever Yes
//Proyecto: Prelaboratorio 
//Descripción: Contador 4 bits interrupciones
// Hardware: ATMega328P
//Created: 25-02-2024
//**
//Encabezado
//***
 
.include "M328PDEF.inc"
.cseg 
.org 0x00
	JMP MAIN		//Vector Reset
.org 0x0006 //PCINT0
	JMP ISR_PCINT0
.org 0x0020
	JMP ISR_TIMER_OVF

MAIN:
//****
//Configuración de la Pila
//****
LDI R16,LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17
//*****
//TABLA DE VALORES
//*****
			TABLA7SEG: .DB 0B01000000, 0B01111001, 0b00100100, 0b00110000, 0b00011001, 0b0010010, 0b0000010, 0b01111000, 0b00000000, 0b00010000
							//0            1           2         3             4          5           6          7           8         9 
				
//*****
SETUP:
	//HABILITAR PRESCALER
	LDI R16, 0B10000000
	LDI R16, (1<< CLKPCE)
	STS CLKPR, R16

	//Frecuencia del sistema
	LDI R16, 0B00000001
	STS CLKPR, R16

	//CONFIGURAR SALIDAS
	LDI r16, 0XFF
	OUT DDRD, R16		//Salida 7 seg	
	;OUT PORTC, R16		//Se activan los pullups 
	;ldi R16, 0XFF
	OUT DDRC, R16
	LDI R20, 0
	sts UCSR0B,  R20	//UTILIZAR EL PD0 Y PD1
	

	
	LDI R16, 0b00000111		// SALIDAS PB4 Y PB5(PINES DE CONTROL DE LOS 7 SEG)
	OUT PORTB, R16
	LDI R16, 0B00011000
	OUT DDRB, R16
	 ; Habilitar la interrupción PCINT0
    ldi r16, (1 << PCIE0)      ; Habilitar la interrupción de cambio de pin
    STS PCICR, r16              ; Establecer el registro de control de interrupción de cambio de pin
    
	LDI r16, (1<<PCINT1)|(1 << PCINT2) ; Habilitar la interrupción  con PCINT0 (PCINT8)
    STS PCMSK0, r16 
	
	LDI R24, 0B11101111
	OUT PORTB, R24
	
	SEI
	LDI R19, 0
	LDI R17, 0 
	LDI R28, 0
	LDI R25, 0
	LDI R22,0
	LDI R21,0 

//****************************************************
//TIMER0
//****************************************************
Timer0:
	LDI R23, 0
	OUT TCCR0A, R23		//NORMAL


	LDI R23, (1<<CS02) |  (1<<CS00)
	OUT TCCR0B, R23						//PRESCALER DE 1024
	
	LDI R23, 178 	;CARGAR EL VALOR DE DESBORDAMIENTO
	OUT TCNT0, R23			;Cargar el valor incial 
	
	LDI R23, (1<<TOIE0)
	STS TIMSK0, R23

	JMP LOOP
//Reiniciar el 7seg de las unidades

REINICIOU:
	LDI R22,0
	INC R21
	JMP LOOP
//REINICIAR EL CONTADOR DE LOS MS 
//AUMENTAR EL REGISTRO DEL CONTADOR DE LAS UNIDADES
REINICIOMS:
	INC R22
	LDI R23, 0
	JMP LOOP

LOOP:
	CPI R22, 10			//Compararacion para aumentar las decenas
	BREQ REINICIOU
	CPI R23, 100		//COMO EL TIMER0 ESTA 10MS Y SE NECESITAN 1000MS
	BREQ REINICIOMS		//CUANDO HAYAN PASADO LOS 1000MS CARGAR EL VALOR

	CALL Mult7seg		
	LDI R24, 0B11100111			//HABILITAR LOS PINES CORRIESPONDIENTES AL CONTROL DEL 7 SEGMENOTS
	OUT PORTB, R24
	LDI R24, 0B11110111
	OUT PORTB, R24

VD7seg:
	LDI ZH, HIGH(TABLA7SEG<<1)
	LDI ZL, LOW(TABLA7SEG<<1)
	ADD	ZL, R21

	LPM R25, Z
	OUT PORTD, R25

	CALL Mult7seg

	LDI R24, 0B11101111
	OUT PORTB, R24
	LDI R24, 0B11101111
	OUT PORTB, R24
VU7SEG:

	LDI ZH, HIGH(TABLA7SEG<<1)
	LDI ZL, LOW(TABLA7SEG<<1)
	ADD	ZL, R22
	LPM R25, Z
	OUT PORTD, R25
	CALL Mult7seg

	//Limite de las decenas de llegar hasta 6
LimiteD:
	CPI R21, 6
	BREQ REINICIO7SEG
	JMP LOOP

//Tiempo de espera()
Mult7seg:
	LDI R19, 255
//Serie de delays quemados xd
D1:
	DEC R19
	BRNE d1
	LDI R19, 255

D2:
	DEC R19
	BRNE d2
	LDI R19, 255
D3:
	DEC R19
	BRNE D3
	LDI R19, 255
D4:
	DEC R19
	BRNE d4

RET


//REINICAR AMBOS 7 SEGMENTOS
REINICIO7SEG:
	CALL Mult7seg 
	LDI R21, 0
	LDI R22,0
	JMP LOOP
	
	rjmp LOOP
//**********************************************
//SUBRUTINA DE ISR TIMEROVF
//**********************************************

ISR_TIMER_OVF:
	PUSH R17				// Se guarda en pila el r16
	IN R17, SREG
	PUSH R17
	
	LDI R16, 178			//TIMER0 10MS
	OUT TCNT0, R16
	SBI TIFR0, TOV0
	INC r23					//CONTADOR DE LOS MS

fin_interrupcion2:
	POP R17
	OUT SREG, R17
	POP R17
	RETI 

//*************************
//***********************
// Subrutina de ISR PCINT0
//***********************
ISR_PCINT0:
	PUSH R16				// Se guarda en pila el r16
	
	IN R16, SREG
	PUSH R16				//Se guarda en pila el registro sreg
	 
	IN R20, PINB			// LEER EL PINB
	;ANDI R20, 00000110		//Solo importan los valores de PB1 Y PB2
	;CPI R20, 2
	;brne Boton2
	SBRC R20, PB2			//El boton del pinb2 va ser para decrementar

	JMP BOTON2

	DEC R18					
	JMP Fin_interrupcion

Boton2:
	;cpi r20, 4
	;brne Fin_interrupcion

	SBRC R20, PB1		//El boton para incrementar esta conectado al pb1
	JMP Fin_interrupcion
	INC R18
Fin_interrupcion:
	//FILTRO PARA QUE EL CONTADOR SEA SOLO DE 4 BITS
	ANDI R18, 0B00001111
	OUT PORTC, R18
	SBI PCIFR, PCIF0
	POP R16
	OUT SREG, R16
	POP R16
	RETI

