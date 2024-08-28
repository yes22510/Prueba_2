#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "I2C/I2C.h"
#include "LCD/LCD.h"
#include "UART/UART.h"

#define DS3231_ADDR 0x68  // Dirección del DS3231
#define SLAVE1_ADDR 0x02  // Dirección del primer esclavo
#define SLAVE2_ADDR 0x01  // Dirección del segundo esclavo

char buffer[128];
char buffer1[16];
char buffer2[16];
uint8_t Abierto, day, ventilador, dato;

// Variables globales para almacenar el estado anterior de la puerta
uint8_t estadoAnteriorPuerta = 255;  // Inicialmente 255 para asegurarse de que siempre envíe el primer valor

void DS3231_Init(void) {
    // No es necesario inicializar nada en específico para el DS3231.
}

void DS3231_ReadTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds) {
    uint8_t data[3];
    uint8_t addr = 0x00;  // Dirección del registro de la hora en el DS3231

    I2C_Master_Start();
    I2C_Master_Write(DS3231_ADDR << 1);  // Enviar dirección del esclavo con bit de escritura
    I2C_Master_Write(addr);  // Especificar que se comenzará a leer desde el registro de segundos

    I2C_Master_Start();  // Repetir START para cambiar a modo lectura
    I2C_Master_Write((DS3231_ADDR << 1) | 1);  // Enviar dirección del esclavo con bit de lectura

    for (uint8_t i = 0; i < 3; i++) {
        if (i < 2) {
            I2C_Master_Read(&data[i], 1);  // Leer con ACK
        } else {
            I2C_Master_Read(&data[i], 0);  // Leer con NACK
        }
    }
    
    I2C_Master_Stop();

    *seconds = ((data[0] >> 4) * 10) + (data[0] & 0x0F);
    *minutes = ((data[1] >> 4) * 10) + (data[1] & 0x0F);
    *hours   = ((data[2] >> 4) * 10) + (data[2] & 0x0F);
}

void portonUltra() {
    uint8_t result = read_from_slave(SLAVE2_ADDR, &Abierto);
    int abierto2 = (int)(Abierto);
    if (result == 0) {
        if (abierto2 == 1) {
            strcpy(buffer2, "Abierto");
        } else {
            strcpy(buffer2, "cerrado");
        }
    } else {
        strcpy(buffer2, "error");
    }
}

void enviarDatosAlESP32(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    static uint8_t minutoAnterior = 255;

    // Enviar la hora por UART solo si ha cambiado el minuto
    if (minutes != minutoAnterior) {
        minutoAnterior = minutes;
        snprintf(buffer, sizeof(buffer), "Hora:%02d:%02d:%02d\n", hours, minutes, seconds);
        writeUART_Text(buffer);
    }

    // Enviar el estado de la puerta solo si ha cambiado
    uint8_t estadoActualPuerta = (Abierto == 1) ? 1 : 0;
    if (estadoActualPuerta != estadoAnteriorPuerta) {
        estadoAnteriorPuerta = estadoActualPuerta;
        snprintf(buffer, sizeof(buffer), "Puerta:%s\n", buffer2);
        writeUART_Text(buffer);
    }
}

void uartGarita() {
    if (UCSR0A & (1 << RXC0)) {  // Verificar si hay datos disponibles para leer
        char comando = receivedChar();

        // Ignorar espacios en blanco o caracteres de fin de línea
        if (comando == '\n' || comando == '\r' || comando == ' ') {
            return;
        }
        
        snprintf(buffer, sizeof(buffer), "Recibido desde ESP32: %c\n", comando);
        writeUART_Text(buffer);

        // Procesar el comando recibido y enviar la bandera correspondiente al esclavo
        if (comando == '1') {
            send_to_slave(SLAVE2_ADDR, 'A');  // Envía la bandera 'A' al esclavo 1 si el comando es 1
            writeUART_Text("Bandera 'A' enviada al esclavo 1\n");
        }
        else if (comando == '0') {
            send_to_slave(SLAVE2_ADDR, 'B');  // Envía la bandera 'B' al esclavo 1 si el comando es 0
            writeUART_Text("Bandera 'B' enviada al esclavo 1\n");
        }
    }
}

int main(void) {
    // Inicialización de periféricos
    I2C_Master_Init(100, 4);
    initLCD8bits();  // Inicializar el LCD en modo 8 bits
    initUART(9600, 0, 0, 0, 0, 1, 8, 103);  // Inicializar UART a 9600 baudios
    
    uint8_t hours, minutes, seconds;

    DS3231_Init();  // Inicializar el DS3231

    while (1) {
        portonUltra();
        DS3231_ReadTime(&hours, &minutes, &seconds);

        // Mostrar la hora en el LCD
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
        LCD_Set_Cursor(1, 2); // Posiciona el cursor en la primera línea del LCD
        LCD_Write_String("        "); // Limpia la línea
        LCD_Set_Cursor(1, 2); // Posiciona el cursor nuevamente
        LCD_Write_String(buffer); // Muestra la hora formateada
        LCD_Set_Cursor(1, 1); // Posiciona el cursor nuevamente
        LCD_Write_String("Tiempo:"); // Muestra la etiqueta

        // Muestra el estado del portón
        LCD_Set_Cursor(11, 2); // Posiciona el cursor en la segunda línea del LCD
        LCD_Write_String("        "); // Limpia la línea
        LCD_Set_Cursor(11, 2); // Posiciona el cursor nuevamente
        LCD_Write_String(buffer2); // Muestra el estado del portón
        LCD_Set_Cursor(11, 1); // Posiciona el cursor nuevamente
        LCD_Write_String("Puerta:"); // Muestra la etiqueta

        // Enviar los datos al ESP32
        enviarDatosAlESP32(hours, minutes, seconds);

        // Procesar el comando recibido del ESP32 y enviarlo al esclavo adecuado
        uartGarita();

        _delay_ms(1000);  // Esperar 1 segundo antes de la siguiente lectura
    }
}
