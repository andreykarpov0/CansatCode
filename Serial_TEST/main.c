/*
 * Serial_TEST.c
 *
 * Created: 03.06.2021 16:40:37
 * Author : akarp
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "MySerial.h"

int main(void) {
	Serial_Begin();
	
	uint8_t num = 98;
	
    while (1) {
		num  = Serial_Read();
		Serial_Write_char(num);
		_delay_ms(1);
    }
}