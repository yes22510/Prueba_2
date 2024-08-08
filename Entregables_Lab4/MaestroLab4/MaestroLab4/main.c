/*
 * Author : yesev
 */ 
#define F_CPU 16000000 // Establece la frecuencia del microcontrolador en Hz
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCD/LCD.h"
#include "I2C/I2C.h"

#define SENSOR1_ADDR 0x02 // Dirección del primer sensor
#define SENSOR2_ADDR 0x01 // Dirección del segundo sensor

uint8_t sensor1_data, sensor2_data;
char display_buffer1[16];
char display_buffer2[16];

void ReadSensor1() {
    uint8_t status = read_from_slave(SENSOR1_ADDR, &sensor1_data);

    if (status == 0) {
        // Si la lectura es exitosa
        snprintf(display_buffer1, sizeof(display_buffer1), " %u", sensor1_data);
    } else {
        // Maneja el error de lectura
        snprintf(display_buffer1, sizeof(display_buffer1), "Error %u", status);
    }
}

void ReadSensor2() {
    uint8_t status = read_from_slave(SENSOR2_ADDR, &sensor2_data);

    if (status == 0) {
        // Si la lectura es exitosa
        snprintf(display_buffer2, sizeof(display_buffer2), " %u", sensor2_data);
    } else {
        // Maneja el error de lectura
        snprintf(display_buffer2, sizeof(display_buffer2), "Error %u", status);
    }
}

int main(void) {
    // Inicializa el LCD
    initLCD8bits(); // Asegúrate de que esta función configure el LCD correctamente
    // Configura el bus I2C con frecuencia de 100 kHz y prescaler 1
    I2C_Master_Init(100, 4);

    sei(); // Habilita las interrupciones globales

    while (1) {
        // Lee el dato del primer sensor
        ReadSensor1();
        LCD_Set_Cursor(1, 2); // Posiciona el cursor en la primera línea del LCD
        LCD_Write_String("        "); // Limpia la línea
        LCD_Set_Cursor(1, 2); // Posiciona el cursor nuevamente
        LCD_Write_String(display_buffer1); // Muestra el valor o el error
        LCD_Set_Cursor(2, 1); // Posiciona el cursor nuevamente
        LCD_Write_String("Sensor1:"); // Muestra el valor o el error

        // Lee el dato del segundo sensor
        ReadSensor2();
        LCD_Set_Cursor(9, 2); // Posiciona el cursor en la segunda línea del LCD
        LCD_Write_String("        "); // Limpia la línea
        LCD_Set_Cursor(9, 2); // Posiciona el cursor nuevamente
        LCD_Write_String(display_buffer2); // Muestra el valor o el error
        LCD_Set_Cursor(9, 1); // Posiciona el cursor nuevamente
        LCD_Write_String("Sensor2:"); // Muestra el valor o el error

        _delay_ms(500); // Espera antes de la siguiente lectura
    }
}
