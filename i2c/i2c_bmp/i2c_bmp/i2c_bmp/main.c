#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "bmp280_i2c.h"
static int USART_PutChar(char c, FILE *stream)
{
	if (c == '\n')
	USART_PutChar('\r', stream);
	USART_SendByte(c);
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

int main(){
	uint8_t bmp280_connection, bmp280_data_received=0, chid;
	int32_t temperature;
	float pressure, alt;
	USART_Init ( 9600 );
	I2CInit(F_CPU, 100000);

	

	printf("\nBMP280 I2C Demo\n");
	printf("\nTwooooooooo\n");
	bmp280_connection = BMP280_begin(0x76);
	printf("\nThreeeeeeeeeeeee\n");
	printf(bmp280_connection);
	if(bmp280_connection == 1)
	{
		printf("\nBMP280 connection OK\n");
	}
	else
	{
		printf("\nBMP280 connection not OK\n");
	}
	while(1){
		bmp280_data_received = BMP280_receiveData(&temperature,&pressure,&alt);
		if (bmp280_data_received ==1){
			printf("temp = %d\n press = %f\n alt = %f\n",(int)temperature, pressure, alt);
			}else{
			printf("BMP280 measurement not OK\n");
		}
		chid = BMP280_read8(BMP280_REGISTER_CHIPID);
		printf("chid = %.2X\n", chid);
		_delay_ms(1000);

	}
}
