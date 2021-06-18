/*
 * GccApplication2.c
 *
 * Created: 07.05.2021 18:19:32
 * Author : akarp
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	DDRD = 0b10000000;
	while(1)
	{
		PORTD = (1<<PD7);
		_delay_ms(10000);
		PORTD = (0<<PD7);
		_delay_ms(10000);
	}
}