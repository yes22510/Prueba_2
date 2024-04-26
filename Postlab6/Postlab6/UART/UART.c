/*
 * UART.c
 *  Author: Ever Yes
 */ 

#include "UART.h"

void initUART(int baud_rate, int turbo, int multiprocesador, int sincrono, int paridad, int bit_stop, int cantidad_caracteres, int UBRR0_valor) {
	// Configurar pines RX y TX
	DDRD &= ~(1 << DDD0); // RX como entrada
	DDRD |= (1 << DDD1);  // TX como salida

	// Configurar UCSR0A
	UCSR0A = 0;
	if (turbo == 1) {
		UCSR0A |= (1 << U2X0); // Modo turbo
	}
	if (multiprocesador == 1) {
		UCSR0A |= (1 << MPCM0); // Habilitar modo multiprocesador
	}

	// Configurar UCSR0B
	UCSR0B = (1 << RXCIE0) | (1 << UDRIE0) | (1 << TXEN0) | (1 << RXEN0); // Habilitar RX, TX e interrupciones

	// Configurar UCSR0C
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Tamaño de datos predeterminado a 8 bits
	if (sincrono == 1) {
		UCSR0C |= (1 << UMSEL00); // Modo síncrono
	}
	if (paridad == 1) {
		UCSR0C |= (1 << UPM01); // Paridad par
		} else if (paridad == 2) {
		UCSR0C |= (1 << UPM01) | (1 << UPM00); // Paridad impar
	}
	if (bit_stop == 2) {
		UCSR0C |= (1 << USBS0); // 2 bits de parada
	}

	// Configurar tamaño de caracteres
	switch (cantidad_caracteres) {
		case 6:
		UCSR0C |= (1 << UCSZ00); // 6 bits de datos
		break;
		case 7:
		UCSR0C |= (1 << UCSZ01); // 7 bits de datos
		break;
		case 8:
		UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos
		break;
		case 9:
		UCSR0C |= (1 << UCSZ02) | (1 << UCSZ01) | (1 << UCSZ00); // 9 bits de datos
		break;
	}

	// Configurar UBRR0
	UBRR0 = UBRR0_valor;
}

void writeUART_car(char caracter) {
	while (!(UCSR0A & (1 << UDRE0))); // Esperar a que el buffer de transmisión esté vacío
	UDR0 = caracter; // Enviar el caracter
}

void writeUART_Text(char* texto) {
	while (*texto != '\0') {
		writeUART_car(*texto); // Enviar cada caracter del texto
		texto++;
	}
}

uint8_t receivedChar(void) {
	while (!(UCSR0A & (1 << RXC0))); // Esperar a que se reciba un caracter
	return UDR0; // Devolver el caracter recibido
}
