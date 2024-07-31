/*
 * Timer0.c
 *
 * Created: 16/04/2024 23:02:38
 *  Author: angel
 */ 
#include "TIMER2.h"

void init_Timer2(int modo, int preescaler){
			TCCR2A=0;
			TCCR2B=0;
			TIMSK2=0;
	if (modo==0){
		//Habilitar interrupciones por overflow
		TIMSK2 |= (1<<TOIE2);
	}
	if (modo==1){
		//MODO CTC
		TCCR2A |= (1<<WGM21);
		//Habilitar interrupciones por comparación
		TIMSK2 |= (1<<OCIE2A)|(1<<OCIE2B);
	}
	
	//Definimos el preescaler
	switch(preescaler){
		case 1:
		TCCR2B |= (1<<CS20);
		break;
		case 8:
		TCCR2B |= (1<<CS21);
		break;
		case 64:
		TCCR2B |= (1<<CS22);
		break;
		case 256:
		TCCR2B |= (1<<CS22)|(1<<CS21);
		break;
		case 1024:
		TCCR2B |= (1<<CS22)|(1<<CS20)|(1<<CS21);
		break;
	}
}

void CTC_Timer2A(uint8_t valor){
	OCR2A = valor;
}