/*
 * UART.c
 *
 * Created: 23/04/2024 22:58:50
 *  Author: angel
 */ 

#include "UART.h"

void initUART(int baud_rate, int turbo, int multiprocesador, int sincrono, int paridad, int bit_stop, int cantidad_caracteres,int UBRR0_valor){
	//RX ENTRADA
	DDRD &= ~(1<<DDD0);
	//TX SALIDA
	DDRD |= (1<<DDD1);
	
	UCSR0A=0;
	//Modo turbo
	if (turbo==1){
		UCSR0A |= (1<<U2X0);
	}
	//Habilitar multiprocesador
	if (multiprocesador==1){
		UCSR0A |= (1<<MPCM0);
	}

	//Interrupciones
	UCSR0B=0;
	//RX interrupcion recepcion (1<<RXCIE0)
	//TX interrupcion transmision (1<<TXCIE0)
	// Interrupcion de datas (1<<UDRIE0)
	UCSR0B |= (1<<RXCIE0)|(1<<UDRIE0)|(1<<UDRIE0);
	
	//Habilitar recepcion y envio
	//RX RECEPCION TX ENVIO
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	

	UCSR0C=0;
	if (sincrono==1){
		UCSR0C |= (1<<UMSEL00);
	}
	if (paridad==1){
		//Par
		UCSR0C |= (1<<UPM01);
		} else if (paridad==2){
		//Impar
		UCSR0C |= (1<<UPM01)|(1<<UPM00);
	}
	if (bit_stop==2){
		UCSR0C |= (1<<USBS0);
	}
	
	switch (cantidad_caracteres){
		case 5:
		
		break;
		case 6:
		UCSR0C |= (1<<UCSZ00);
		break;
		case 7:
		UCSR0C |= (1<<UCSZ01);
		break;
		case 8:
		UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
		break;
		case 9:
		UCSR0C |= (1<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00);
		break;
	}
	
	//UBRR0 = ((Foscilo)/(16 x BAUD))-1
	UBRR0=UBRR0_valor;
	
}

void writeUART_car(char caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0=caracter;
}

void writeUART_Text(char* texto){
	uint8_t i;
	for (i=0; texto[i]!='\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0=texto[i];
	}
}

uint8_t receivedChar(void){
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}