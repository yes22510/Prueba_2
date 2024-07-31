/*
 * PMW0.c
 *
 * Created: 14/04/2024 14:09:44
 *  Author: angel
 */ 

#include "PMW0.h" 

void init_PMW0A(int orientacion,int modo,int preescaler){
	//Puerto de salida
	DDRD |= (1<<DDD6);
	TCCR0A=0;
	TCCR0B=0;
	
	//Definimos la orientaci�n en invertido o no invertido
	if (orientacion==0){
		//No invertido
		TCCR0A |= (1<<COM0A1);
		}else{
		//invertido
		TCCR0A |= (1<<COM0A1) | (1<<COM0A0);
	}
	//Definimos el modo de operaci�n FAST o Phase Correct
	if (modo==0){
		//FAST
		TCCR0A |= (1<<WGM00) | (1<<WGM01);
		} else{
		//Phase Correct
		TCCR0A |= (1<<WGM00);
	}
	//Definimos el preescaler
	switch(preescaler){
		case 0:
		TCCR0B |= (1<<CS00);
		break;
		case 8:
		TCCR0B |= (1<<CS01);
		break;
		case 64:
		TCCR0B |= (1<<CS01)|(1<<CS00);
		break;
		case 256:
		TCCR0B |= (1<<CS02);
		break;
		case 1024:
		TCCR0B |= (1<<CS02)|(1<<CS00);
		break;
		
	}
	
}

void init_PMW0B(int orientacion){
	//Puerto de salida
	DDRD |= (1<<DDD5);
	//Definimos la orientaci�n en invertido o no invertido
	if (orientacion==0){
		//No invertido
		TCCR0A |= (1<<COM0B1);
		}else{
		//invertido
		TCCR0A |= (1<<COM0B1) | (1<<COM0B0);
	}
	
}

void duty_cycleA(int duty){
	OCR0A = duty;
}

void duty_cycleB(int duty){
	OCR0B = duty;
}
