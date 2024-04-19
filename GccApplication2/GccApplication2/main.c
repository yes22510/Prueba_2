//***//Encabezado
//Universidad del Valle de Guatemala
//IE 2023: Programación de Microcontroladores
//Autor: Ever Yes
//Proyecto: Prelaboratorio
//Descripción: Servo
// Hardware: ATMega328P
//**

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "T1PWM/T1PWM.h"
#include <avr/interrupt.h>
int Lectura;
float adc;
//************************************************************
// SETUP
//************************************************************

int main(void)
{
 initADC();

 // Configurar pin del LED como salida
 DDRB |= (1 << LED_PIN);

 // Configurar Timer2 para generar una interrupción cada 10ms (aprox.)
 OCR2A = 156; // Para una frecuencia de 10ms con prescaler de 64
 TCCR2A |= (1 << WGM21); // Modo CTC (Clear Timer on Compare Match)
 TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler de 1024
 TIMSK2 |= (1 << OCIE2A); // Habilita interrupción por comparación con OCR2A


 // Habilitar interrupciones globales
 sei();

 // Iniciar primera conversión ADC
 ADCSRA |= (1 << ADSC);

	while (1)
	{
		
		
		if (counter == intensity) {
			// Encender el LED
			//PORTB |= (1 << LED_PIN);
			PORTB &= ~(1 << LED_PIN);
			counter = 0;
			} else {
			// Apagar el LED
			PORTB |= (1 << LED_PIN);
			//PORTB &= ~(1 << LED_PIN);
		}
			ADMUX = (ADMUX & 0xF8) | 0x06; // Selecciona el canal A6
			ADCSRA |= (1 << ADSC);  // Inicia una conversión ADC
			_delay_ms(10);           // Pequeña espera entre actualizaciones del ADC
			
			// Inicia una conversión ADC para el canal B (A4)
			ADMUX = (ADMUX & 0xF8) | 0x04; // Selecciona el canal A4
			ADCSRA |= (1 << ADSC);  // Inicia una conversión ADC
			_delay_ms(10);
		}
		
		

	}
}
void initADC() {
// Configura la referencia de voltaje a AVCC con capacitor externo en AREF
ADMUX |= (1 << REFS0);
ADMUX &= ~(1 << REFS1);
ADMUX |= (1 << ADLAR);

ADCSRA |= (1 << ADEN) | (1 << ADIE);
ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}
ISR(ADC_vect) {
	if ((ADMUX & 0x07) == 0x04) { // Si es el canal B (A4)
		adc_value_ = ADCH; 
		servo_set(adc_value_ * 100); 
		} else { // Si es el canal A (A6)
		adc_value_Servo = ADCH;
		servo_set(adc_value_Servo); 
	}