/*
 * ADC.h
 *
 * Created: 15/04/2024 7:27:42
 *  Author: angel
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void init_ADC(int referencia,int justificacion,int preescaler);
uint8_t readADC(int canal);
void readADC_ISR(int canal);




#endif /* ADC_H_ */