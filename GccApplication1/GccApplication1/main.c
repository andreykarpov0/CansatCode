#include <stdio.h>
#include <avr/io.h>

#include "atmega_bmp280_lib.h"
//#include "uart.h"

#define F_CPU 8000000

int main(void)
{	
	int32_t temperature;
	int32_t pressure;
	int32_t altitude;
	uint8_t error_code=0;
	
	int32_t var1, var2;

	// Initialize UART0
	USART_Init ( 9600 );
	
	// Initialize TWI
	i2cSetBitrate(1000);

	// Initialize BMP180
	BMP280_Begin(&error_code);

	if (error_code > 0) {
		printf("Error code: %d \n",error_code);
		return -1;
	}

    while(1)
    {
		bmp280Convert(&temperature, &pressure, &error_code);
		altitude = bmp280CalcAltitude(pressure);

		var1 = temperature/10;
		var2 = temperature - var1*10;
		printf("Temperature: %d.%d\n",(double) var1,(double) var2);

		printf("Pressure: %d Pa\n",(double) pressure);

		var1 = altitude/100;
		var2 = (altitude - var1*100)/10;
		printf("Altitude: %d.%d\n\n",(double) var1,(double) var2);

		_delay_ms(200);
    }
}
