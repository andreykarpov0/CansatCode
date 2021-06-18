#include <stdio.h>
#include <avr/io.h>

#include "atmega_bmp280_lib.h"
#include "uart.h"

int main(void)
{	
	int32_t temperature;
	int32_t pressure;
	int32_t altitude;
	uint8_t error_code=0;
	
	int32_t var1, var2;

	// Initialize UART0
	uart_init();
	
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
		printf("Temperature: %ld.%ld\n", var1, var2);

		printf("Pressure: %ld Pa\n", pressure);

		var1 = altitude/100;
		var2 = (altitude - var1*100)/10;
		printf("Altitude: %ld.%ld\n\n", var1, var2);

		_delay_ms(200);
    }
}
