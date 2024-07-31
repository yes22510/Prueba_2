/*
 * LCD16X2.h
 *
 *  Author: Ever
 */  



#ifndef LCD8BITS_H_
#define LCD8BITS_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>


//FUNCIÓN PARA INICIALIZAR LCD EN 8 BITS
void initLCD8bits(void);
//FUNCIÓN PARA ENVIAR COMANDOS
void LCD_CMD(char a);
//FUNCIÓN PARA COLOCAR EN EL PUERTO UN VALOR
void LCD_Port(char a);
//FUNCIÓN PARA ENVIAR UN CARACTER
void LCD_Write_Char(char c);
//FUNCIÓN PARA ENVIAR UNA CADENA
void LCD_Write_String(char *a);
//FUNCIÓN PARA CORRER A LA DERECHA
void LCD_Shift_Right(void);
//FUNCIÓN PARA CORRER A LA IZQUIERDA
void LCD_Shift_Left(void);
//ESTABLECER EL CURSOR
void LCD_Set_Cursor(char c, char f);

#endif /* LCD8BITS_H_ */