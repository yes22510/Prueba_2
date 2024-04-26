/*
 * ADC.c
 *  Author: Ever Yes
 */ 
#include "ADC.h"
void init_ADC(int referencia, int justificacion, int preescaler) {
	// Configuración de la referencia del ADC
	if (referencia == 0) {
		// Voltaje de referencia interno de 5V.
		ADMUX |= (1 << REFS0);
		} else if (referencia == 1) {
		// Voltaje de referencia interno de 1.1V.
		ADMUX |= (1 << REFS0) | (1 << REFS1);
	}

	// Configuración de la justificación (IZQUIERDA O DERECHA)
	if (justificacion == 0) {
		// Justificación izquierda
		ADMUX |= (1 << ADLAR);
		} else if (justificacion == 1) {
		// Justificación derecha (predeterminado)
		ADMUX &= ~(1 << ADLAR);
	}

	// Configuración del preescalador
	ADCSRA |= preescaler;

	// Encendiendo el ADC
	ADCSRA |= (1 << ADEN);
}

uint8_t readADC(int canal) {
	// Seleccionar el canal de entrada
	ADMUX = (ADMUX & 0xF8) | (canal & 0x07);

	// Iniciar la conversión
	ADCSRA |= (1 << ADSC);

	// Esperar hasta que la conversión esté completa
	while (ADCSRA & (1 << ADSC)) {
		// Esperar
	}

	// Leer y devolver el valor del ADC
	return ADCH;
}

void readADC_ISR(int canal) {
	// Habilitar la interrupción del ADC
	ADCSRA |= (1 << ADIE);

	// Seleccionar el canal de entrada
	ADMUX = (ADMUX & 0xF8) | (canal & 0x07);

	// Iniciar la conversión
	ADCSRA |= (1 << ADSC);
}
