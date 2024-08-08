/*
 * EsclavoContador.c
 *
 * Author : yesev
 */ 


#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C/I2C.h"

// NUEVAS DEFINICIONES DE PINES
#define BTN_A_PIN PIND2
#define BTN_B_PIN PIND3
#define LAMP1_PIN PINB0
#define LAMP2_PIN PINB1
#define LAMP3_PIN PINB2
#define LAMP4_PIN PINB3
#define I2C_ADDR 0x02

// VARIABLES GLOBALES
volatile uint8_t COUNT = 0;
volatile uint8_t received_data = 0;
volatile uint8_t prev_count = 0;

// VARIABLES DE ANTIRREBOTE
volatile uint8_t btnA_state = 0;
volatile uint8_t btnB_state = 0;
volatile uint8_t debounce_count = 0;

// PROTOTIPOS DE FUNCIONES
void setupPinChangeInterrupt(void);
void setupTimer1(void);

// ISR para TWI (I2C)
ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;

    switch (status) {
        case 0x60:
        case 0x70:
            TWCR |= (1 << TWINT);
            break;
        case 0x80:
        case 0x90:
            received_data = TWDR;
            TWCR |= (1 << TWINT);
            break;
        case 0xA8:
        case 0xB8:
            TWDR = COUNT;
            TWCR |= (1 << TWINT) | (1 << TWEA);
            break;
        case 0xC0:
        case 0xC8:
            TWCR |= (1 << TWINT) | (1 << TWEA);
            break;
        default:
            TWCR |= (1 << TWINT) | (1 << TWSTO);
            break;
    }
}

// Configuración de interrupciones de cambio de pin
void setupPinChangeInterrupt(void) {
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
    PCICR |= (1 << PCIE2);
}

// Configuración del Timer1
void setupTimer1(void) {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11);
    OCR1A = 1999;
    TIMSK1 = (1 << OCIE1A);
}

// ISR para cambios en PCINT2
ISR(PCINT2_vect) {
    debounce_count = 1;
}

// ISR para comparación de Timer1
ISR(TIMER1_COMPA_vect) {
    if (debounce_count) {
        debounce_count++;
        if (debounce_count >= 20) {
            debounce_count = 0;

            uint8_t btnA_current = PIND & (1 << BTN_A_PIN);
            uint8_t btnB_current = PIND & (1 << BTN_B_PIN);

            if (!(btnA_current) && btnA_state == 0) {
                COUNT++;
                if (COUNT > 15) {
                    COUNT = 15;
                }
                btnA_state = 1;
            }
            if (btnA_current) {
                btnA_state = 0;
            }

            if (!(btnB_current) && btnB_state == 0) {
                COUNT--;
                if (COUNT > 15) {
                    COUNT = 0;
                }
                btnB_state = 1;
            }
            if (btnB_current) {
                btnB_state = 0;
            }

            if (COUNT != prev_count) {
                PORTB = COUNT;
                prev_count = COUNT;
            }
        }
    }
}

// Función principal
int main(void) {
    cli();

    DDRD &= ~((1 << BTN_A_PIN) | (1 << BTN_B_PIN));
    PORTD |= (1 << BTN_A_PIN) | (1 << BTN_B_PIN);

    DDRB |= (1 << LAMP1_PIN) | (1 << LAMP2_PIN) | (1 << LAMP3_PIN) | (1 << LAMP4_PIN);

    I2C_Slave_Init(I2C_ADDR);
    setupPinChangeInterrupt();
    setupTimer1();
    sei();

    while (1) {
        // Bucle principal vacío
    }
}
