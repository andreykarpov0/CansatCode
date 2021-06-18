#define F_CPU 8000000
#define BAUD 9600
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include "uart.h"
#include "i2c_avr.h"
#include "adxl345_i2c.h"
static int USART_PutChar(char c, FILE *stream)
{
	if (c == '\n')
	USART_PutChar('\r', stream);
	USART_SendByte(c);
	return 0;
}
static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

void Sensors_Init()
{
	I2CInit(100000);
	int i = ADXL345_begin(ADXL345_ALTERNATIVE_ADDRESS);
	_delay_ms(1000);
	if (i==1){
		printf ("ADXL INIT OK");
		} else {
		printf ("ADXL INIT NOT OK");
	}
}
int main( void )
{
	int x = 0, y = 0, z = 0;
	float module_g = 0;
	USART_Init ( BAUD );
	stdout = &mystdout; 
	Sensors_Init();

	while (1)
	{
		ADXL345_readXYZ_raw(&x, &y, &z);
		float x1 = x*0.041744;
		float y1 = y*0.041744;
		float z1 = z*0.041744;
		printf("x = %f\n ", x1);
		printf("y = %f\n ", y1);
		printf("z = %f\n ", z1);
		module_g = sqrt(pow(x1,2)+pow(y1,2)+pow(z1,2));
		printf("module_g = %f\n ", module_g);
		_delay_ms(500);
	}
	return 0;
}