/*
 * Timer0.h
 *
 * Created: 16/04/2024 23:02:27
 *  Author: angel
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

#include <stdint.h>
#include <avr/io.h>

void init_Timer0(int modo, int preescaler);
void CTC_Timer0A(uint8_t valor);
void CTC_Timer0B(uint8_t valor);





#endif /* TIMER0_H_ */