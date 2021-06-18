#define  F_CPU 8000000
#define  FOSC 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "bmp280_i2c.h"
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

void USART_INIT()
{
	DDRD = 0b10000000;
	UBRR0H = 0;
	UBRR0L = 51;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	PORTD = (1<<PD7);
	_delay_ms(1000);
}
int USART_RECIVE()
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
void USART_TRANSMITT(uint8_t data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}
void STRING_TRANSMITT(char string[])
{
	for(int i = 0;string[i] != 0;i++)
	{
		USART_TRANSMITT((uint8_t)string[i]);
	}
}
int main()
{
	uint8_t bmp280_connection, bmp280_data_received=0, chid;
	int32_t temperature;
	float pressure, alt;
	int x = 0, y = 0, z = 0;
	
	
	I2CInit(100000);
	Sensors_Init();
	USART_INIT(9600);
	bmp280_connection = BMP280_begin(0x76);
	PORTD = (0<<PD7);
	char str[] = "Output: ";
	
	stdout = &mystdout;
	printf("111/n");
	
	if(bmp280_connection == 1)
	{
		
	}
	else
	{
		
	}
	
	while(1)
	{
		ADXL345_readXYZ_raw(&x, &y, &z);
		float x1 = x*0.041744;
		float y1 = y*0.041744;
		float z1 = z*0.041744;
		bmp280_data_received = BMP280_receiveData(&temperature,&pressure,&alt);
		int data = USART_RECIVE();
		
		if(data=='1')
		{
			PORTD = (1<<PD7);
		}
		else
		{
			PORTD = (0<<PD7);
		}
		
		if (bmp280_data_received ==1)
		{
			printf("temp = %d\n press = %f\n alt = %f\n",(int)temperature, pressure, alt);
		}
		else
		{
			
		}
		
		chid = BMP280_read8(BMP280_REGISTER_CHIPID);
		printf("x = %f y = %f z = %f/n", x1, y1, z1);
		printf("chid = %.2X\n", chid);
		printf("temperature(ds18b20) = %d", data);
		USART_TRANSMITT(0x0d);
		USART_TRANSMITT(0x0a);
		STRING_TRANSMITT(str);
		USART_TRANSMITT(data);
		_delay_ms(100);
	}
	return 0;
}