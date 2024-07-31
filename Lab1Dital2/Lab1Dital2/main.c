/*
 * Lab1Dital2.c
 *
 * Created: 12/07/2024 06:42:19
 * Author : yesev
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int contJ2;
int contJ1;
// Map of the 7-segment display segments for numbers 0-5
uint8_t segment_map[6] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101  // 5
};

volatile uint8_t start_sequence = 0;

// External interrupt service routine
ISR(INT0_vect) {
    start_sequence = 1;
}

//void display_digit(uint8_t digit) {
    // Output the segments to the display
    //PORTB = segment_map[digit];
//}

int main(void) {
	//Configuración de interrupciones
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0)| (1 << PCINT1)| (1 << PCINT2);
	DDRD |= 0xFF;
	//PORTD = 0;
	UCSR0B = 0;
	DDRB |= 0b00111000;
	PORTB |= 0b00111111;
	
	DDRC |= 0xFF;
    // Set port B as output for the 7-segment display
    DDRD = 0xFF;
    PORTD = 0x00;

    // Set PD2 (INT0) as input with pull-up resistor
    //DDRB &= ~(1 << PB0);
    //PORTB |= (1 << PB0);

    // Enable external interrupt on INT0 (PD2) with falling edge trigger
    //EICRA |= (1 << ISC01);
    //EIMSK |= (1 << INT0);

    // Enable global interrupts
    sei();

    while (1) {
        if (start_sequence==1) {
			contJ1=0;
			PORTC = 0b00000000;
			PORTC &= ~(1 << PC4); // Apagar PC4
			PORTB &= ~(1 << PB3); // Apagar PB3
			PORTB &= ~(1 << PB4); // Apagar PB4
			PORTB &= ~(1 << PB5); // Apagar PB5
            // Start the countdown sequence
            for (int8_t i = 5; i >= 0; i--) {
                PORTD = segment_map[i];
                _delay_ms(1000); // 1-second delay
				// Ejecutar código adicional cuando la secuencia llegue a 0
				// Aquí va tu código adicional
            }
            // Reset the flag
            start_sequence = 0;
			contJ1=0;
			contJ2=0;
			// if (contJ1 == 5) {
				//PORTC= 0b00001111;
				//PORTD = segment_map[1];
				// Ejecutar código adicional cuando la secuencia llegue a 0
				// Aquí va tu código adicional
				//}
        }
		// Asumiendo que PORTC está configurado correctamente como salida
		// y contJ1 está definido e inicializado adecuadamente

		switch (contJ1) {
    case 0:
        PORTC &= ~(1 << PC0); // Apagar PC0
        PORTC &= ~(1 << PC1); // Apagar PC1
        PORTC &= ~(1 << PC2); // Apagar PC2
        PORTC &= ~(1 << PC3); // Apagar PC3
        break;
    case 1:
        PORTC |= (1 << PC0);  // Encender PC0
        PORTC &= ~(1 << PC1); // Apagar PC1
        PORTC &= ~(1 << PC2); // Apagar PC2
        PORTC &= ~(1 << PC3); // Apagar PC3
        break;
    case 2:
        PORTC &= ~(1 << PC0); // Apagar PC0
        PORTC |= (1 << PC1);  // Encender PC1
        PORTC &= ~(1 << PC2); // Apagar PC2
        PORTC &= ~(1 << PC3); // Apagar PC3
        break;
    case 3:
        PORTC &= ~(1 << PC0); // Apagar PC0
        PORTC &= ~(1 << PC1); // Apagar PC1
        PORTC |= (1 << PC2);  // Encender PC2
        PORTC &= ~(1 << PC3); // Apagar PC3
        break;
    case 4:
        PORTC &= ~(1 << PC0); // Apagar PC0
        PORTC &= ~(1 << PC1); // Apagar PC1
        PORTC &= ~(1 << PC2); // Apagar PC2
        PORTC |= (1 << PC3);  // Encender PC3
        break;
    case 5:
        PORTC |= (1 << PC0);  // Encender PC0
        PORTC |= (1 << PC1);  // Encender PC1
        PORTC |= (1 << PC2);  // Encender PC2
        PORTC |= (1 << PC3);  // Encender PC3
        PORTD = segment_map[1];
		contJ2=0;
		PORTC &= ~(1 << PC4); // Apagar PC4
		PORTB &= ~(1 << PB3); // Apagar PB3
		PORTB &= ~(1 << PB4); // Apagar PB4
		PORTB &= ~(1 << PB5); // Apagar PB5
        break;
    default:
        PORTC |= (1 << PC0);  // Encender PC0
        PORTC |= (1 << PC1);  // Encender PC1
        PORTC |= (1 << PC2);  // Encender PC2
        PORTC |= (1 << PC3);  // Encender PC3
        contJ1 = 5;
        break;
}



		switch (contJ2) {
			case 0:
				PORTC &= ~(1 << PC4); // Apagar PC4
				PORTB &= ~(1 << PB3); // Apagar PB3
				PORTB &= ~(1 << PB4); // Apagar PB4
				PORTB &= ~(1 << PB5); // Apagar PB5
				break;
			case 1:
				PORTC &= ~(1 << PC4); // Apagar PC4
				PORTB &= ~(1 << PB3); // Apagar PB3
				PORTB |= (1 << PB4);  // Encender PB4
				PORTB &= ~(1 << PB5); // Apagar PB5
				break;
			case 2:
				PORTC &= ~(1 << PC4); // Apagar PC4
				PORTB |= (1 << PB3);  // Encender PB3
				PORTB &= ~(1 << PB4); // Apagar PB4
				PORTB &= ~(1 << PB5); // Apagar PB5
				break;
			case 3:
				PORTC &= ~(1 << PC4); // Apagar PC4
				PORTB &= ~(1 << PB3); // Apagar PB3
				PORTB &= ~(1 << PB4); // Apagar PB4
				PORTB |= (1 << PB5);  // Encender PB5
				break;
			case 4:
				PORTC |= (1 << PC4);  // Encender PC4
				PORTB &= ~(1 << PB3); // Apagar PB3
				PORTB &= ~(1 << PB4); // Apagar PB4
				PORTB &= ~(1 << PB5); // Apagar PB5
				break;
			case 5:
				PORTC |= (1 << PC4);  // Encender PC4
				PORTB |= (1 << PB3);  // Encender PB3
				PORTB |= (1 << PB4);  // Encender PB4
				PORTB |= (1 << PB5);  // Encender PB5
				PORTD = segment_map[2];
				contJ1=0;
				PORTC &= ~(1 << PC0); // Apagar PC0
				PORTC &= ~(1 << PC1); // Apagar PC1
				PORTC &= ~(1 << PC2); // Apagar PC2
				PORTC &= ~(1 << PC3); // Apagar PC3
				break;
			default:
				PORTC |= (1 << PC4);  // Encender PC4
				PORTB |= (1 << PB3);  // Encender PB3
				PORTB |= (1 << PB4);  // Encender PB4
				PORTB |= (1 << PB5);  // Encender PB5
				contJ2 = 5;
				break;
		}


		//PORTC= contJ1;
    }
}
ISR(PCINT0_vect){
	if ((PINB&(1<<PINB0))==0) //Reviso el botón de decremento
	start_sequence=1;
	if ((PINB&(1<<PINB1))==0) //Reviso el botón de incremento
	contJ1++;
	if ((PINB&(1<<PINB2))==0) //Reviso el botón de decremento
	contJ2++;
}
