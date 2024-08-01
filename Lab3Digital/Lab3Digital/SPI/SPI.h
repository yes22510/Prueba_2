/*
 * IncFile1.h
 *
 * Created: 26/07/2024 12:36:24
 *  Author: yesev
 */ 


#ifndef SPI_M_H_
#define SPI_M_H_
#include <avr/io.h>
#include <stdint.h>

typedef enum
{
	SPI_MASTER_OSC_DIV2   = 0b01010000,
	SPI_MASTER_OSC_DIV4   = 0b01010001,
	SPI_MASTER_OSC_DIV8   = 0b01010010,
	SPI_MASTER_OSC_DIV16  = 0b01010011,
	SPI_MASTER_OSC_DIV32  = 0b01010100,
	SPI_MASTER_OSC_DIV64  = 0b01010101,
	SPI_MASTER_OSC_DIV128 = 0b01010110,
	SPI_SLAVE_SS          = 0b01000000
}Spi_Type;

typedef enum
{
	SPI_DATA_ORDER_MSB = 0b00000000,
	SPI_DATA_ORDER_LSB = 0b00100000
}Spi_Data_Order;

typedef enum
{
	SPI_CLOCK_IDLE_HIGH = 0b00001000,
	SPI_CLOCK_IDLE_LOW  = 0b00000000
}Spi_Clock_Polarity;

typedef enum
{
	SPI_CLOK_FIRST_EDGE = 0b00000000,
	SPI_CLOCK_LAST_EDGE = 0b00000100
}Spi_Clock_Phase;

void SpiInit(Spi_Type,Spi_Data_Order,Spi_Clock_Polarity, Spi_Clock_Phase);
void SpiWrite(uint8_t dat);
unsigned SpiDataReady();
uint8_t SpiRead(void);
#endif /* SPI_M_H_ */