/*
 * main.c
 *  Author: Ever Yes
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ADC/ADC.h"
#include "UART/UART.h"
// Definiciones de pines
#define LED_DDR DDRD
#define LED_PORT PORTB
#define LED1_PIN DDD2
#define LED2_PIN DDD3

#define UART_BAUD_RATE 9600

// Función para mostrar el menú y obtener la opción del usuario
uint8_t menu(void){
	uint8_t opcion=0;
	char mensaje[]="1) Leer pot\n2) Enviar ASCII\n";
	writeUART_Text(mensaje);
	opcion=receivedChar();
	return opcion;
}

int main(void)
{
	// Inicialización de los periféricos
	LED_DDR |= (1<<LED1_PIN)|(1<<LED2_PIN);
	init_ADC(0,0,128);
	initUART(UART_BAUD_RATE,0,0,0,0,1,8,103);
	
	uint8_t modo=0;
	uint8_t valorADC=0;
	char bufferTX[10];
	char ASCII_P[255];
	
	// Bucle principal
	while(1)
	{
		// Mostrar el menú y obtener la opción
		modo=menu();
		
		if (modo==0b0110001){
			// Modo 1: Leer potenciómetro
			
			// Leer el valor del potenciómetro
			valorADC=readADC(6);
			
			// Formatear el valor como una cadena y enviarlo por UART
			snprintf(bufferTX, sizeof(bufferTX), "%d\n", valorADC);
			writeUART_Text(bufferTX);
		}
		else if (modo==0b0110010){
			// Modo 2: Enviar caracter ASCII
			
			// Mostrar mensaje en la terminal
			snprintf(ASCII_P, sizeof(ASCII_P), "Caracter a mostrar\n");
			writeUART_Text(ASCII_P);
			
			// Leer el caracter de la terminal
			buffer=receivedChar();
			
			// Procesar el caracter y enviarlo a los puertos correspondientes
			valor = (buffer & 0b00111111) << 2;
			LED_PORT = valor;
			valor = (buffer & 0b11000000) >> 6;
			LED_PORT = valor;
		}
		
		// Pequeña espera antes de la siguiente iteración
		_delay_ms(10);
	}
}
