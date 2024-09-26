/*+
 * Lab6_Atmega.c
 *
 * Created: 20/09/2024 11:03:20
 * Author : yesev
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "UART/UART.h"

#define F_CPU 16000000UL

// Definir pines para los botones
#define BUTTON1_PIN PD2
#define BUTTON2_PIN PD3
#define BUTTON3_PIN PD4
#define BUTTON4_PIN PD5
#define BUTTON5_PIN PD6
#define BUTTON6_PIN PD7

void setup() {
	// Configurar los pines de los botones como entradas
	DDRD &= ~(1 << BUTTON1_PIN); // Bot�n 1
	DDRD &= ~(1 << BUTTON2_PIN); // Bot�n 2
	DDRD &= ~(1 << BUTTON3_PIN); // Bot�n 3
	DDRD &= ~(1 << BUTTON4_PIN); // Bot�n 4
	DDRD &= ~(1 << BUTTON5_PIN); // Bot�n 5
	DDRD &= ~(1 << BUTTON6_PIN); // Bot�n 6

	// Habilitar pull-up interno para los botones
	PORTD |= (1 << BUTTON1_PIN);
	PORTD |= (1 << BUTTON2_PIN);
	PORTD |= (1 << BUTTON3_PIN);
	PORTD |= (1 << BUTTON4_PIN);
	PORTD |= (1 << BUTTON5_PIN);
	PORTD |= (1 << BUTTON6_PIN);

	// Inicializar UART
	initUART(9600, 0, 0, 0, 0, 1, 8, 103); // Velocidad de 9600 baudios
}

void loop() {
	// Leer cada bot�n y enviar una letra por UART
	if (!(PIND & (1 << BUTTON1_PIN))) {
		writeUART_car('A'); // Bot�n 1 presionado, enviar 'A'
		_delay_ms(300); // Evitar rebote
	}
	if (!(PIND & (1 << BUTTON2_PIN))) {
		writeUART_car('B'); // Bot�n 2 presionado, enviar 'B'
		_delay_ms(300);
�	}
	if (!(PIND & (1 << BUTTON3_PIN))) {
		writeUART_car('C'); // Bot�n 3 presionado, enviar 'C'
		_delay_ms(300);
	}
	if (!(PIND & (1 << BUTTON4_PIN))) {
		writeUART_car('D'); // Bot�n 4 presionado, enviar 'D'
		_delay_ms(300);
	}
	if (!(PIND & (1 << BUTTON5_PIN))) {
		writeUART_car('E'); // Bot�n 5 presionado, enviar 'E'
		_delay_ms(300);
	}
	if (!(PIND & (1 << BUTTON6_PIN))) {
		writeUART_car('F'); // Bot�n 6 presionado, enviar 'F'
		_delay_ms(300);
	}
}

int main(void) {
	setup();
	while (1) {
		loop(); // Llamar al loop principal continuamente
	}
}
