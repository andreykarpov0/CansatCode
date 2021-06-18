/*
 * CanSatFinal.c
 *
 * Created: 25.04.2021 15:57:05
 * Author : Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "SPI.h"
#include "MyTWI.h"
#include "BMP.h"
#include "ADXL.h"
#include "ds18b20/ds18b20.h"
#include "nRF_24.h"
#include "MySerial.h"

int main(void) {
	spi_init();
	TWI_Init(F_CPU, 100000UL);
	BMP280_begin(BMP_ADDR_DEFAULT);
	ADXL345_Begin(ADXL_ADDR_SEC);
	Serial_Begin();
	
	
	RF24_Begin(25);
	_delay_ms(500);
	RF24_Setup_Transmitter();
	
	uint8_t serialPackage[5];
	uint8_t Package[RF24_PackageSize] = { 0 };
	int16_t adxl_x, adxl_y, adxl_z, DStemperature, UVData;
	int32_t BMtemperature, press, alt;
	int DHTTemperature, DHTHumidity;
	float pressure, altitude;
	
	while(1){
		for(int i = 0; i < 5; ++i)
		{
			serialPackage[i] = Serial_Read();
			//Serial_Write_char(serialPackage[i]);
		}
		ds18b20convert( &PORTB, &DDRB, &PINB, ( 1 << PINB7 ), 0 );
		_delay_ms( 100 );
		
		if(ds18b20read( &PORTB, &DDRB, &PINB, ( 1 << PINB7 ), 0, &DStemperature ) == DS18B20_ERROR_OK) {
			DStemperature *= 6.25; DStemperature += 5500;
			
			Package[0] = ((DStemperature >> 8) & 0xFF);
			Package[1] = (DStemperature & 0xFF);
		}
		if(BMP280_receiveData(&BMtemperature, &pressure, &altitude) == 1) {
			press = pressure * 100; alt = altitude * 10; BMtemperature += 5500;
			
			Package[2] = ((BMtemperature >> 8) & 0xFF);
			Package[3] = (BMtemperature & 0xFF);
			
			Package[4] = ((press >> 16) & 0xFF);
			Package[5] = ((press >> 8) & 0xFF);
			Package[6] = (press & 0xFF);
			
			Package[7] = ((alt >> 8) & 0xFF);
			Package[8] = (alt & 0xFF);
		}
		
		ADXL345_ReadXYZ(&adxl_x, &adxl_y, &adxl_z);
		adxl_x += 10000; adxl_y += 10000; adxl_z += 10000;
		
		Package[10] = ((adxl_x >> 8) & 0xFF);
		Package[11] = (adxl_x & 0xFF);
		
		Package[12] = ((adxl_y >> 8) & 0xFF);
		Package[13] = (adxl_y & 0xFF);
		
		Package[14] = ((adxl_z >> 8) & 0xFF);
		Package[15] = (adxl_z & 0xFF);
		
		Package[16] = serialPackage[0];
		Package[17] = serialPackage[1];
		Package[18] = serialPackage[2];
		
		Package[19] = serialPackage[3];
		Package[20] = serialPackage[4];
		
		
		RF24_Send(Package, RF24_PackageSize);
		_delay_ms(100);
	}
	return 0;
}