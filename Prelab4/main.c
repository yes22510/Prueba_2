//***
	qa//Encabezado
//Universidad del Valle de Guatemala
//IE 2023: Programación de Microcontroladores
//Autor: Ever Yes
//Proyecto: Prelaboratorio 
//Descripción: ContadOR 8 BITS
// Hardware: ATMega328P
//**

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void initADC(void);		
uint8_t mylist[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
int Lectura;
int cont8;
int Prim7seg;
int Seg7seg;

int main(void)
{

	cli();
	initADC();
	sei();
	 //Configuración de interrupciones
	 PCICR |= (1 << PCIE0);
	 PCMSK0 |= (1 << PCINT0)| (1 << PCINT1);
	DDRD |= 0xFF;
	PORTD = 0;
	UCSR0B = 0;
	DDRB |= 0b00111100;
	
  
	DDRC |= 0xFF;
    cont8 = 0;
    while (1) 
	{
		ADCSRA |= (1<<ADSC);
		if(cont8 > 255)
		cont8 = 255;
		if (cont8 < 0)
		cont8 = 0;
		PORTD = cont8;
		
		_delay_ms(5);
		PORTB = (1<<PORTB3);
		Prim7seg = Lectura & 0x0F;
		PORTC = mylist[Prim7seg];
		if (Prim7seg == 0 || Prim7seg == 1 || Prim7seg == 7 || Prim7seg == 12) 
		{
		 PORTB |= (0<<PORTB5);
		} else {
		 PORTB |= (1<<PORTB5);
	}

	_delay_ms(4);
		PORTB = (1<<PORTB2);
		Seg7seg = (Lectura>>4) & 0x0F;
		if (Seg7seg == 0 || Seg7seg == 1 || Seg7seg == 7 || Seg7seg == 12) {
			PORTB |= (0<<PORTB5);
			} else {
			PORTB |= (1<<PORTB5);
		}

		PORTC = mylist[Seg7seg];
		
		if (cont8 < Lectura){
			PORTB |= (1<<PORTB4);
		}
		if (cont8 > Lectura){
			PORTB |= (0<<PORTB4);
		}
    }
		
}


void initADC(void)
{
	// Seleccion de Canal ADC0
	ADMUX = 7;
	
	// Utilizando AVCC = 5V internos
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	// Justificacion a la Izquierda
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	
	ADCSRA |= (1<<ADEN);
	//Habilitamos las interrupciones
	ADCSRA |= (1<<ADIE);
	
	// Habilitamos el Prescaler de 128
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	// Habilitando el ADC
	ADCSRA |= (1<<ADEN);
}

ISR(ADC_vect){
	Lectura = ADCH;
	ADCSRA |= (1<<ADIF);

}
ISR(PCINT0_vect){
	if ((PINB&(1<<PINB1))==0) //Reviso el botón de incremento
	cont8++;
	if ((PINB&(1<<PINB0))==0) //Reviso el botón de decremento
	cont8--;
}