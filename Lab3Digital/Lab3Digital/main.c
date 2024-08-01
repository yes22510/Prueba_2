/*004
 * Lab3Digital.c
 *
 * Created: 26/07/2024 11:15:09
 * Author : yesev
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
//#include "LCD8b/LCD8b.h"
//#include "ADC/ADC.h"
#include "UART/UART.h"
#include "SPI/"
int valorPOT2, valorPOT1;
void leer_potenciometros() {	
	// Leer respuesta del esclavo
	char buffer[20];
	for (int i = 0; i < sizeof(buffer) - 1; i++) {
		buffer[i] = uart_receive_char();
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			break;
		}
	}
}
void enviarValoresUART() {
	valorPOT1 = (int)(valorPOT1);
	valorPOT2 = (int)(valorPOT2);
	UART_send_string("\n\r");
	snprintf(buffer, sizeof(buffer), "P1: %d \r\n", valorPOT1);
	UART_send_string(buffer);
	_delay_ms(1);
	snprintf(buffer, sizeof(buffer), "P2: %d \r\n", valorPOT2);
	UART_send_string(buffer);
}
void mostrar_contador_leds() {
	// Aquí iría tu lógica para manejar el contador de LEDs
	 // Leer 3 caracteres desde UART
	 for (i = 0; i < 3; i++) {
		 buffer[i] = receivedChar();
	 }
	 buffer[3] = '\0'; // Finalizar la cadena de texto

	 received_value = (uint8_t)atoi(buffer);

	 // Asignar los primeros 6 bits a PORTD (PD2-PD7)
	 PORTD = (PORTD & 0b00000011) | (received_value << 2);

	 // Asignar los bits 7 y 8 a PORTB (PB0 y PB1)
	 PORTB = (PORTB & 0b11111100) | ((received_value >> 6) & 0b00000011);

	 // _delay_ms(500); // Pausa opcional si es necesario
 }

}

int main() {
	uint8_t received_value =0;
	char buffer[4];
	uint8_t i = 0;

	// Inicializar UART con baud rate 9600 (UBRR0_valor depende del reloj del microcontrolador)
	initUART(9600, 0, 0, 0, 0, 1, 8, 103);

	// Configurar PD2 a PD7 como salidas
	DDRD = 0b11111100;
	// Configurar PB0 y PB1 (que corresponden a PD8 y PD9) como salidas
	DDRB |= (1 << DDB0) | (1 << DDB1);
	XSpiInit(SPI_MASTER_OSC_DIV128, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOK_FIRST_EDGE);
	while (1) {
		// Mostrar menú
		uart_send_string("Menú Principal\n");
		uart_send_string("1. Leer Potenciómetros\n");
		uart_send_string("2. Mostrar Contador de LEDs\n");
		uart_send_string("Selecciona una opción: ");

		// Leer opción del usuario
		option = uart_receive_char();
		uart_send_char(option);  // Echo de la opción seleccionada
		uart_send_char('\n');

		switch (option) {
			case '1':
			leer_potenciometros();
			break;
			case '2':
			mostrar_contador_leds();
			break;
			default:
			uart_send_string("Opción no válida\n");
			break;
		}

		_delay_ms(1000);  // Espera antes de mostrar el menú de nuevo
	}
    return 0;
}
