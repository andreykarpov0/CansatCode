/*
 * NRF_24.c
 *
 * Created: 07.02.2021 12:01:40
 * Author : Egor
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include "NRF_24.h"
#include "MySerial.h"

static uint8_t package[RF24_PackageSize] = { 0, 7, 9, 8, 5, 7, 2, 6, 44, 26, 7, 9, 8, 5, 7, 2, 6, 44, 2, 6, 26, 21, 22, 23, 44, 25, 26, 27, 28, 29, 30, 31 };

int main(void) {
	Serial_Begin();
	spi_init();
	
	if (RF24_Begin(25)) {
		Serial_Write_char('1');
	}
	else {
		Serial_Write_char('0');
	}
	
	RF24_Setup_Transmitter();
	
    while (1) {
		RF24_Send(package, RF24_PackageSize);
		_delay_ms(500);
    }
}