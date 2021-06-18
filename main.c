#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "bmp280_i2c.h"
#include "adxl345_i2c.h"
#include "nrf.h"


#define F_CPU 8000000// Clock Speed

static int8_t USART_PutChar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

static int8_t USART_PutChar(char c, FILE *stream)
{
	if (c == '\n')
		USART_PutChar('\r', stream);
	USART_SendByte(c);
	return 0;
}
// Помещает пакет в очередь отправки.
// buf - буфер с данными, size - длина данных (от 1 до 32)
uint8_t send_data(float * buf, uint8_t size) {
	radio_deassert_ce(); // Если в режиме приёма, то выключаем его
	uint8_t conf = radio_readreg(CONFIG);
	// Сбрасываем бит PRIM_RX, и включаем питание установкой PWR_UP
	uint8_t status = radio_writereg(CONFIG, (conf & ~(1 << PRIM_RX)) | (1 << PWR_UP));
	if (status & (1 << TX_FULL_STATUS))  // Если очередь передатчика заполнена, возвращаемся с ошибкой
	return 0;
	if (!(conf & (1 << PWR_UP))) // Если питание не было включено, то ждём, пока запустится осциллятор
	_delay_ms(2);
	radio_write_buf(W_TX_PAYLOAD, buf, size); // Запись данных на отправку
	radio_assert_ce(); // Импульс на линии CE приведёт к началу передачи
	_delay_us(15); // Нужно минимум 10мкс, возьмём с запасом
	radio_deassert_ce();
	return 1;
}
void check_radio() {
	if (!radio_is_interrupt()) // Если прерывания нет, то не задерживаемся
	return;
	uint8_t status = radio_cmd(NOP);
	radio_writereg(STATUS, status); // Просто запишем регистр обратно, тем самым сбросив биты прерываний
	
	if (status & ((1 << TX_DS) | (1 << MAX_RT))) { // Завершена передача успехом, или нет,
		if (status & (1 << MAX_RT)) { // Если достигнуто максимальное число попыток
			radio_cmd(FLUSH_TX); // Удалим последний пакет из очереди
			on_send_error(); // Вызовем обработчик
		}
		if (!(radio_readreg(FIFO_STATUS) & (1 << TX_EMPTY))) { // Если в очереди передатчика есть что передавать
			radio_assert_ce(); // Импульс на линии CE приведёт к началу передачи
			_delay_us(15); // Нужно минимум 10мкс, возьмём с запасом
			radio_deassert_ce();
			} else {
			uint8_t conf = radio_readreg(CONFIG);
			radio_writereg(CONFIG, conf & ~(1 << PWR_UP)); // Если пусто, отключаем питание
		}
	}
	uint8_t protect = 4; // В очереди FIFO не должно быть более 3 пакетов. Если больше, значит что-то не так
	while (((status & (7 << RX_P_NO)) != (7 << RX_P_NO)) && protect--) { // Пока в очереди есть принятый пакет
		radio_cmd(FLUSH_RX); // во всех случаях выкидываем пришедший пакет.
		status = radio_cmd(NOP);
	}
}

int main()
{
	uint8_t bmp280_connection, bmp280_data_received=0, chid, nSensors, i, error;
	int32_t temperature, mxRes;
	uint16_t a0;
	int16_t decicelsius;
	float pressure, alt;
	float zero = 0;
	float x, y, z;
	float module_g;

	spi_init();
	radio_init();
	while (!radio_start()) {
		_delay_ms(1000);
	}
	USART_Init ( 9600 );
	I2CInit(F_CPU, 100000);
	_delay_ms(2); 
	radio_assert_ce();
	
	stdout = &mystdout;

	printf("\nBMP280 I2C Demo\n");
	bmp280_connection = BMP280_begin(BMP280_DEFAULT_ADDRESS);
	if(bmp280_connection==1)
	{
		printf("BMP280 connection OK\n");
	}
	else
	{
		printf("BMP280 connection not OK\n");
		return 0;
	}
	printf("\nDS18X20 1-Wire-Reader Demo\n");
	printf("-------------------------------------------\n");
	nSensors = search_sensors();
	printf( "%i DS18X20 Sensor(s) available:\n", nSensors);
	int i = ADXL345_begin(ADXL345_DEFAULT_ADDRESS);
		if (i==1)
		{
			printf ("ADXL INIT OK");
		} 
		else 
		{
			printf ("ADXL INIT NOT OK");
		}
	for(int i=0;i<10;i++){
		BMP280_receiveData(&temperature,&pressure,&alt);
		zero += alt;
	}
	zero/=10;

	while(1)
	{
		check_radio();
		bmp280_data_received = BMP280_receiveData(&temperature,&pressure,&alt);
		if (bmp280_data_received ==1)
		{
			printf("tempBMP280 = %ld\n press = %.4f\n alt = %.4f\n",temperature, pressure, alt - zero);
		}
		else
		{
			printf("BMP280 measurement not OK\n");
		}
		ADXL345_readXYZ(&x,&y,&z);
		printf("x = %.6f\n ", x);
		printf("y = %.6f\n ", y);
		printf("z = %.6f\n ", z);
		module_g = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
		printf("module_g = %.6f\n ", module_g);
		for ( i = nSensors; i > 0; i-- ) {
			printf("Sensor# %i \n",i);
			print_address(gSensorIDs[i-1]);
			DS18X20_set_resolution(&gSensorIDs[i-1][0], DS18B20_12_BIT);

			if ( DS18X20_start_meas( DS18X20_POWER_EXTERN,&gSensorIDs[i-1][0]) == DS18X20_OK ) 
			{
				_delay_ms( DS18B20_TCONV_12BIT );
				if (DS18X20_read_decicelsius(&gSensorIDs[i-1][0], &decicelsius)	== DS18X20_OK ) 
				{
					printf("tempDS18X20 = %i\n", decicelsius );
				} 
				else 
				{
					printf( "CRC Error (lost connection?)" );
					error++;
				}
				if (DS18X20_read_maxres(&gSensorIDs[i-1][0], &mxRes)	== DS18X20_OK ) 
				{
					printf("temp_maxREs = %.4f\n", mxRes*0.0001 );
				} 
				else 
				{
					printf( "CRC Error (lost connection?)" );
					error++;
				}

			}
			else 
			{
				printf( "Start meas. failed (short circuit?)\n" );
				error++;
			}
		}
		send_data({decicelsius, mxRes, x, y, z, temperature, pressure}, 7);
		_delay_ms(200);

	}
}