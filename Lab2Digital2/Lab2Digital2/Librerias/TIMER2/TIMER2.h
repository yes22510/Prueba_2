/*
 * TIMER2.h
 *
 * Created: 17/04/2024 10:20:09
 *  Author: angel
 */ 


#ifndef TIMER2_H_
#define TIMER2_H_

#include <stdint.h>
#include <avr/io.h>

void init_Timer2(int modo, int preescaler);
void CTC_Timer2A(uint8_t valor);




#endif /* TIMER2_H_ */