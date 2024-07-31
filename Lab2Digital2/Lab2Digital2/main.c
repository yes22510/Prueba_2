/*
 * Lab2Digital2.c
 *
 * Created: 19/07/2024 10:59:22
 * Author : yesev
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCD_8bits/LCD_8bits.h"
#include "ADC/ADC.h"
#include "UART/UART.h"

#define INIT_COUNTER_VALUE 0

int main(void) {
    // Configuración de pines de ADC como entradas
    DDRC &= ~((1 << DDC6) | (1 << DDC0));
    PORTC |= (1 << PORTC1) | (1 << PORTC0);

    // Inicializar el LCD, el ADC y la UART
    initLCD8bits();
    init_ADC(0, 0, 128);  // Referencia AVcc, justificación a la izquierda, preescaler 128
    initUART(9600, 0, 0, 0, 0, 1, 8, 103);

    char buffer[16];
    int adc_value1, adc_value2;
    int voltage1, voltage2;
    int counter = 0;
    
    while (1) {
        // Leer valores de ADC
        adc_value1 = readADC(1);  // Canal 0
        adc_value2 = readADC(0);  // Canal 1
        
        // Convertir valores ADC a voltaje
        voltage1 = adc_value1 * (5.0 / 255.0) * 100;
        voltage2 = adc_value2 * (5.0 / 255.0) * 100;
        
        // Mostrar voltaje1 en el LCD
        LCD_Set_Cursor(1, 1);
        LCD_Write_String(" S1 ");
        LCD_Set_Cursor(1, 2);
        snprintf(buffer, sizeof(buffer), " %d.%.02d ", voltage1 / 100, voltage1 % 100);
        LCD_Write_String(buffer);
        writeUART_Text(" S1");
        writeUART_Text(buffer);

        // Mostrar voltaje2 en el LCD
        LCD_Set_Cursor(7, 1);
        LCD_Write_String("S2 ");
        LCD_Set_Cursor(7, 2);
        snprintf(buffer, sizeof(buffer), " %d.%.02d ", voltage2 / 100, voltage2 % 100);
        LCD_Write_String(buffer);
        writeUART_Text("\r S2");
        writeUART_Text(buffer);
		
        // Mostrar contador en el LCD
        LCD_Set_Cursor(13, 1);
        LCD_Write_String("S3 ");
		LCD_Set_Cursor(13, 2); // Reposiciona el cursor para mostrar el contador
        snprintf(buffer, sizeof(buffer), " %d", counter);
        LCD_Write_String(buffer);
		//writeUART_Text("S3 ");
        //writeUART_Text(buffer);
		

        // Leer desde la UART si hay datos disponibles
        if (UCSR0A & (1 << RXC0)) {
            char received_char = receivedChar();

            // Actualizar contador basado en el carácter recibido
            if (received_char == '+') {
                counter++;
            } else if (received_char == '-') {
                counter--;
            }
        }

        _delay_ms(500);  // Actualizar cada 500 ms
    }
}

