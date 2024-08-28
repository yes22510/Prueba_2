/*
 * EsclavoMotorDC.c
 *
 * Created: 21/08/2024 01:59:26
 * Author : yesev
 */ 



#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART/UART.h"

#define TRIG_PIN   PD2
#define ECHO_PIN   PD3
#define LED_PIN    PB5
#define MOTOR_PIN  PB0  // Pin para el motor DC


void initUltrasonicSensor() {
    DDRD |= (1 << TRIG_PIN);  // Configura TRIG_PIN como salida
    DDRD &= ~(1 << ECHO_PIN); // Configura ECHO_PIN como entrada
}

uint16_t getDistance() {
    PORTD &= ~(1 << TRIG_PIN);
    _delay_us(2);
    PORTD |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTD &= ~(1 << TRIG_PIN);
    
    while (!(PIND & (1 << ECHO_PIN)));
    
    uint16_t pulse_width = 0;
    while (PIND & (1 << ECHO_PIN)) {
        pulse_width++;
        _delay_us(1);
    }

    uint16_t distance = pulse_width / 58;
    return distance;
}

int main(void) {
    DDRB |= (1 << LED_PIN) | (1 << MOTOR_PIN);  // Configurar LED_PIN y MOTOR_PIN como salida
    
    initUltrasonicSensor();
    initUART(9600, 0, 0, 0, 0, 1, 8, 103);

    char buffer[16];

    while (1) {
        uint16_t distance = getDistance();
        snprintf(buffer, sizeof(buffer), "Distancia: %ucm\n", distance);
        writeUART_Text(buffer);

        if (distance <= 20) {
            PORTB |= (1 << LED_PIN);   // Encender LED
        } else {
            PORTB &= ~(1 << LED_PIN);  // Apagar LED
        }

        // Verificar si el LED está encendido antes de activar el motor
        if (PORTB & (1 << LED_PIN)) {
            PORTB |= (1 << MOTOR_PIN); // Encender motor DC
        } else {
            PORTB &= ~(1 << MOTOR_PIN);// Apagar motor DC
        }

        _delay_ms(500);  // Retardo antes de la siguiente lectura
    }
}
