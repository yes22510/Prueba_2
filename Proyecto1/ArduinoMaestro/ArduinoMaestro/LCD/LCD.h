/*
 * LCD.h
 *
 * Created: 6/08/2024 21:38:09
 *  Author: yesev
 */ 
#ifndef LCD_H_
#define LCD_H_

#include <util/delay.h>
#include <stdint.h> // Para uint8_t

//funcion para inicializar LCD en  8 bits
void initLCD8bits(void);
// funcion para colocar en el puerto un valor
void LCD_port(char a);
//funci�n pata enviar un comando
void LCD_CMD(char a);
// Funci�n para enviar una caracter
void LCD_WriteCaracter(char c);
//funci�n para enviar una cadena
void LCD_Write_String(char * a);
//desplazamiento hacia la derecha
void Lcd_Shift_Right(void);
//desplazamiento hacia la izquierda
void Lcd_Shift_Left(void);
//Establecer cursor
void LCD_Set_Cursor(char c,char f );
// Mover texto a la izquierda
void LCD_Move_Text_Left(char *text, int delay_msx);
// Mover texto a la derecha
void LCD_Move_Text_Right(char *text, int delay_msx);
void delay_ms(unsigned int ms);


#endif /* LCD_H_ */