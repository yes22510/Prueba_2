

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void initUART(int baud_rate, int turbo, int multiprocesador, int sincrono, int paridad, int bit_stop, int cantidad_caracteres,int UBRR0_valor);
void writeUART_car(char caracter);
void writeUART_Text(char* texto);
uint8_t receivedChar(void);




#endif /* UART_H_ */