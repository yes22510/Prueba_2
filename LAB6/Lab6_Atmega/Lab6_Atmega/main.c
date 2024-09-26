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
	DDRD &= ~(1 << BUTTON1_PIN); // Botón 1
	DDRD &= ~(1 << BUTTON2_PIN); // Botón 2
	DDRD &= ~(1 << BUTTON3_PIN); // Botón 3
	DDRD &= ~(1 << BUTTON4_PIN); // Botón 4
	DDRD &= ~(1 << BUTTON5_PIN); // Botón 5
	DDRD &= ~(1 << BUTTON6_PIN); // Botón 6

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
	// Leer cada botón y enviar una letra por UART
	if (!(PIND & (1 << BUTTON1_PIN))) {
		writeUART_car('A'); // Botón 1 presionado, enviar 'A'
		_delay_ms(300); // Evitar rebote
	}
	if (!(PIND & (1 << BUTTON2_PIN))) {
		writeUART_car('B'); // Botón 2 presionado, enviar 'B'
		_delay_ms(300);
º	}
	if (!(PIND & (1 << BUTTON3_PIN))) {
		writeUART_car('C'); // Botón 3 presionado, enviar 'C'
		_delay_ms(300);
	}
	if (!(PIND & (1 << BUTTON4_PIN))) {
		writeUART_car('D'); // Botón 4 presionado, enviar 'D'
		_delay_ms(300);
	}
	if (!(PIND & (1 << BUTTON5_PIN))) {
		writeUART_car('E'); // Botón 5 presionado, enviar 'E'
		_delay_ms(300);
	}
	if (!(PIND & (1 << BUTTON6_PIN))) {
		writeUART_car('F'); // Botón 6 presionado, enviar 'F'
		_delay_ms(300);
	}
}

int main(void) {
	setup();
	while (1) {
		loop(); // Llamar al loop principal continuamente
	}
}
