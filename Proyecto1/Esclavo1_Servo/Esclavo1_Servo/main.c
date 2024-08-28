#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ServoControl/ServoControl.h"
#include "PWM0/PWM0.h"
#include "I2C/I2C.h"

#define TRIG_PIN1  PD2
#define ECHO_PIN1  PD3
#define TRIG_PIN2  PD4
#define ECHO_PIN2  PD5
#define SERVO_PIN1 PB1  // Pin de PWM para controlar el servo 1 (OC1A)
#define SERVO_PIN2 PB2  // Pin de PWM para controlar el servo 2 (OC1B)
#define LED_PIN1   PC0  // LED que controla el servo 1 (A0)
#define LED_PIN2   PC1  // LED que controla el servo 2 (A1)
#define MODE_LED_PIN PC2 // LED indicador de modo (Manual o Automático)
#define SLAVE_ADDR 0x01  // Dirección del esclavo

volatile uint8_t distance = 0;
volatile uint8_t Abierto = 0;
volatile uint8_t dato = 0; // Variable global para almacenar datos recibidos
volatile uint8_t modoManual = 0; // Variable para indicar si estamos en modo manual

/**********************COMUNICACION I2C*********************************/
// Rutina de interrupción del TWI (I2C)
ISR(TWI_vect) {
	uint8_t estado = TWSR & 0xF8;  // Lee los 3 bits superiores del registro de estado

	switch (estado) {
		case 0x60: // SLA+W recibido, ACK enviado
		case 0x70: // SLA+W recibido en modo general, ACK enviado
		TWCR |= (1 << TWINT); // Borra el flag TWINT para continuar
		break;

		case 0x80: // Datos recibidos, ACK enviado
		case 0x90: // Datos recibidos en llamada general, ACK enviado
		dato = TWDR;  // Lee el dato recibido del registro de datos
		TWCR |= (1 << TWINT); // Borra el flag TWINT para continuar
		break;

		case 0xA8: // SLA+R recibido, ACK enviado
		case 0xB8: // Dato transmitido y ACK recibido
		TWDR = Abierto;  // Carga el valor en el registro de datos
		TWCR |= (1 << TWINT) | (1 << TWEA); // Borra el flag TWINT y habilita ACK para el próximo byte
		break;

		case 0xC0: // Dato transmitido y NACK recibido
		case 0xC8: // Último dato transmitido y ACK recibido
		TWCR |= (1 << TWINT) | (1 << TWEA); // Borra el flag TWINT y habilita ACK para el próximo byte
		break;

		default: // Manejo de errores
		TWCR |= (1 << TWINT) | (1 << TWSTO); // Borra el flag TWINT y envía una condición de STOP
		break;
	}
}

void initUltrasonicSensor1() {
    DDRD |= (1 << TRIG_PIN1);
    DDRD &= ~(1 << ECHO_PIN1);
}

void initUltrasonicSensor2() {
    DDRD |= (1 << TRIG_PIN2);  // Configura TRIG_PIN2 como salida
    DDRD &= ~(1 << ECHO_PIN2); // Configura ECHO_PIN2 como entrada
}

uint16_t getDistance1() {
    PORTD &= ~(1 << TRIG_PIN1);
    _delay_us(2);
    PORTD |= (1 << TRIG_PIN1);
    _delay_us(10);
    PORTD &= ~(1 << TRIG_PIN1);

    while (!(PIND & (1 << ECHO_PIN1)));

    uint16_t pulse_width = 0;
    while (PIND & (1 << ECHO_PIN1)) {
        pulse_width++;
        _delay_us(1);
    }

    uint16_t distance = pulse_width / 58;
    return distance;
}

uint16_t getDistance2() {
    PORTD &= ~(1 << TRIG_PIN2);
    _delay_us(2);
    PORTD |= (1 << TRIG_PIN2);
    _delay_us(10);
    PORTD &= ~(1 << TRIG_PIN2);

    while (!(PIND & (1 << ECHO_PIN2)));

    uint16_t pulse_width = 0;
    while (PIND & (1 << ECHO_PIN2)) {
        pulse_width++;
        _delay_us(1);
    }

    uint16_t distance = pulse_width / 58;
    return distance;
}

int main(void) {
    // Configurar los pines de los LEDs como salida (A0, A1 y A2 corresponden a PC0, PC1 y PC2)
    DDRC |= (1 << LED_PIN1) | (1 << LED_PIN2) | (1 << MODE_LED_PIN);
    // Configurar los pines de PWM como salida (PB1 y PB2)
    DDRB |= (1 << SERVO_PIN1) | (1 << SERVO_PIN2);

    // Inicializar sensores ultrasónicos
    initUltrasonicSensor1();
    initUltrasonicSensor2();

    // Inicializar PWM para los servos
    PWM_init();

    // Inicializar I2C como esclavo
    I2C_Slave_Init(SLAVE_ADDR);

    sei();  // Habilitar interrupciones globales

    while (1) {
        uint16_t distance1 = getDistance1();
        uint16_t distance2 = getDistance2();

        // Verificación del modo basado en `dato`
        if (dato == 'A') {
            modoManual = 1;
            PORTC |= (1 << MODE_LED_PIN);  // Enciende el LED de modo manual
        } else if (dato == 'B') {
            modoManual = 0;
            PORTC &= ~(1 << MODE_LED_PIN);  // Apaga el LED de modo manual (modo automático activo)
        }

        // Control del primer LED y enviar señal al maestro
        if (distance1 <= 5) {
			Abierto = 1;
            PORTC |= (1 << LED_PIN1);  // Enciende LED1 (A0)
        } else {
			Abierto = 0;
            PORTC &= ~(1 << LED_PIN1);  // Apaga LED1 (A0)
        }

        if (modoManual == 1) {
            // ** Modo Manual **: Control por Adafruit
            PORTC |= (1 << LED_PIN2);  // Enciende LED2 (A1)
            servo_writeB(100);  // Mover el segundo servo a 90 grados (PB2)
        } else {
            // ** Modo Automático **: Control por sensor ultrasónico
            if (distance2 <= 5) {
                PORTC |= (1 << LED_PIN2);  // Enciende LED2 (A1)
                servo_writeB(100);  // Mover el segundo servo a 90 grados (PB2)
            } else {
                PORTC &= ~(1 << LED_PIN2);  // Apaga LED2 (A1)
                servo_writeB(700);  // Mover el segundo servo a 0 grados (PB2)
            }
        }

        // Movimiento del primer servo basado en el estado del LED1
        if (PORTC & (1 << LED_PIN1)) {
            servo_writeA(700);  // Mover el primer servo a 90 grados (PB1)
        } else {
            servo_writeA(100);  // Mover el primer servo a 0 grados (PB1)
        }

        _delay_ms(500);  // Retardo antes de la siguiente lectura
    }
}
