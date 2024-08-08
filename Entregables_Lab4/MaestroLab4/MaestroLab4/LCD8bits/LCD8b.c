/*
 * LCD16X2.C
 *
 *  Author: Ever
 */ 
 
 #define F_CPU 16000000
 #include "LCD8b.h"
 #include <avr/io.h>
 #include <avr/delay.h>
 //funci�n  para inizialisar LCD en 8 bits

 //funci�n para inicializar LCD en 8 bits
 void initLCD8bits(void) {
	 DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	 PORTD = 0;
	 DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2)|(1 << DDB3);
	 PORTB = 0;

	 _delay_ms(20); // Esperar m�s tiempo para asegurarse de que el LCD se inicializa correctamente

	 // Inicializaci�n en 8 bits
	 LCD_CMD(0x38);
	 _delay_ms(5);
	 LCD_CMD(0x38);
	 _delay_ms(1);
	 LCD_CMD(0x38);
	 _delay_ms(1);

	 // Configuraci�n del LCD
	 LCD_CMD(0x38); // Funci�n set
	 LCD_CMD(0x0C); // Display on, cursor off
	 LCD_CMD(0x01); // Clear display
	 _delay_ms(2);  // Esperar para el clear display
	 LCD_CMD(0x06); // Entry mode set
 }

 // funcion para colocar en el puerto un valor
 void LCD_port(char a) {
	 if (a & 1){
		 //D0 = 1
		 PORTD |= (1<<PORTD4);
		 } else{
		 //D0 =0
		 PORTD &= ~(1<<PORTD4);
	 }
	 /****D1 *****/
	 if (a & 2){
		 //D1 =1
		 PORTD |= (1<<PORTD5);
		 } else{
		 //D1=0
		 PORTD &= ~(1<<PORTD5);
	 }
	 /****D2 *****/
	 if (a & 4){
		 //D0 = 1
		 PORTD |= (1<<PORTD6);
		 } else{
		 //D0 =0
		 PORTD &= ~(1<<PORTD6);
	 }
	 /****D3 *****/

	 if (a & 8){
		 //D1 =1
		 PORTD |= (1<<PORTD7);
		 } else{
		 //D1=0
		 PORTD &= ~(1<<PORTD7);
	 }
	 /****D4 *****/

	 if (a & 16){
		 //D0 = 1
		 PORTB |= (1<<PORTB0);
		 } else{
		 //D0 =0
		 PORTB &= ~(1<<PORTB0);
	 }
	 /****D5 *****/
	 if (a & 32){
		 //D1 =1
		 PORTB |= (1<<PORTB1);
		 } else{
		 //D1=0
		 PORTB &= ~(1<<PORTB1);
	 }
	 /****D6 *****/
	 if (a & 64){
		 //D0 = 1
		 PORTB |= (1<<PORTB2);
		 } else{
		 //D0 =0
		 PORTB &= ~(1<<PORTB2);
	 }
	 /****D7 *****/

	 if (a & 128){
		 //D1 =1
		 PORTB |= (1<<PORTB3);
		 } else{
		 //D1=0
		 PORTB &= ~(1<<PORTB3);
	 }
 }

 //funci�n para enviar un comando
 void LCD_CMD(char a) {
	 PORTB &= ~(1 << PORTB2); // RS=0
	 LCD_port(a);
	 PORTB |= (1 << PORTB3);  // EN=1
	 _delay_us(1);
	 PORTB &= ~(1 << PORTB3); // EN=0
	 _delay_ms(2);
 }

 // Funci�n para enviar un caracter
 void LCD_WriteCaracter(char c) {
	 PORTB |= (1 << PORTB2);  // RS=1
	 LCD_port(c);
	 PORTB |= (1 << PORTB3);  // EN=1
	 _delay_us(1);
	 PORTB &= ~(1 << PORTB3); // EN=0
	 _delay_ms(2);
 }

 //funci�n para enviar una cadena
 void LCD_Write_String(char * a) {
	 int i;
	 for (i = 0; a[i] != '\0'; i++)
	 LCD_WriteCaracter(a[i]);
 }

 //desplazamiento hacia la derecha
 void Lcd_Shift_Right(void) {
	 LCD_CMD(0x1C);
 }

 //desplazamiento hacia la izquierda
 void Lcd_Shift_Left(void) {
	 LCD_CMD(0x18);
 }

 //Establecer cursor
 void LCD_Set_Cursor(char c, char f) {
	 char temp;
	 if (f == 1) {
		 temp = 0x80 + c - 1;
		 } else if (f == 2) {
		 temp = 0xC0 + c - 1;
	 }
	 LCD_CMD(temp);
 }