/*
 *
 *  Author: yesev
 */ 

#ifndef SLAVEI2C_H_
#define SLAVEI2C_H_
#include <stdint.h>

#define TW_SR_SLA_ACK     0x60  // Direcci�n del esclavo recibida con ACK
#define TW_SR_DATA_ACK    0x80  // Datos recibidos con ACK
#define TW_SR_STOP        0xA0  // Condici�n de STOP recibida
#define TW_ST_SLA_ACK     0xA8  // Direcci�n del esclavo para transmisi�n recibida con ACK
#define TW_ST_DATA_ACK    0xB8  // Datos enviados con ACK
#define TW_ST_LAST_DATA   0xC0  // �ltimo dato enviado con ACK
// Inicializa el m�dulo I2C en modo esclavo
void I2C_Slave_Init(uint8_t adress);


#endif /* SLAVEI2C_H_ */