//***//Encabezado
//Universidad del Valle de Guatemala
//IE 2023: Programación de Microcontroladores
//Autor: Ever Yes
//Proyecto: Prelaboratorio
//Descripción: Servo
// Hardware: ATMega328P
//**
#ifndef T1PWM_H_
#define T1PWM_H_
#include <stdint.h>
#include <avr/io.h>
#define invertido 1

void pwm_init(void);
void pwm_sweep(void);
void servo_set(uint16_t deg,uint16_t max_deg);


#endif /* T1PWM_H_ */