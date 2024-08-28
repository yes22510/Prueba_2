/*
 * I2C.c
 *
 * Created: 6/08/2024 21:49:03
 *  Author: yesev
 */ 
#include "I2C.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*****************/
//Funcion para inicializar el esclavo
/*****************/

void I2C_Slave_Init(uint8_t adress){
	DDRC &= ~((1<<DDC4)|(1<<DDC5)); //PINES DE I2C COMO ENTRADAS
	
	//TWAR = adress << 1 & 0b00000010;// se asigna la direccion
	TWAR = (adress <<1| 0x01); //direccion y habilitar la llamada general
	//se habilita la interfaz ACK automatico, se habilita la ISR
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	
}