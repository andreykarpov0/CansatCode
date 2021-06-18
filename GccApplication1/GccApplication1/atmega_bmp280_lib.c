//#################################################################################
//Title:    Library to use BMP280 with ATMEL Atmega family
//Version:  1.0 (13.01.2015)
//Author:   Andrey Koryagin <andre@avislab.com>   http://avislab.com/
//Hardware: any AVR with built-in I2C (TWI), 
//License:  GNU General Public License 
//#################################################################################

#include "atmega_bmp280_lib.h"
#include "twi_lib.h"

uint16_t bmp280ReadShort(uint8_t address, uint8_t* error_code)
{
	uint16_t msb=0;
	uint16_t lsb=0;
	uint16_t data;
	//Send Start Condition
	i2cSendStart();	
	*error_code=i2cWaitForComplete();
	if (*error_code==0){
		*error_code=checki2cReturnCode(TW_START);
	}

	//Tell Salve go in receive mode
	if (*error_code==0){	
		i2cSendByte(BMP280_W);	// write 0xEE
		*error_code=i2cWaitForComplete();
	}
	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MT_SLA_ACK);
	}
	
	//Send read address to Salve
	if (*error_code==0){	
		i2cSendByte(address);	// write register address
		*error_code=i2cWaitForComplete();
	}
	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MT_DATA_ACK);
	}

	//Send Restart to Slave
	if (*error_code==0){	
		i2cSendStart();
		*error_code=i2cWaitForComplete();
	}
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_REP_START);
	}

	//Tell Salve go in send mode
	if (*error_code==0){	
		i2cSendByte(BMP280_R);	// read 0xEF
		*error_code=i2cWaitForComplete();
	}

	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MR_SLA_ACK);
	}

	//Get MSB from Salve
	if (*error_code==0){	
		i2cReceiveByteACK();
		*error_code=i2cWaitForComplete();
	}

	if (*error_code==0){	
		lsb = i2cGetReceivedByte();	// Get LSB result
		*error_code=i2cWaitForComplete();
	}

	//ACKM
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MR_DATA_ACK);
	}

	//Get LSB from Slave
	if (*error_code==0){
		i2cReceiveByteNACK();
		*error_code=i2cWaitForComplete();
	}

	if (*error_code==0){	
		msb = i2cGetReceivedByte();	// Get MSB result
		*error_code=i2cWaitForComplete();
	}

	//NACKM
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MR_DATA_NACK);
	}

	//Send Stop on the Bus
	if (*error_code==0){
		*error_code=i2cSendStop();
	}

	data = (msb << 8) + lsb;
	return data;
}
//----------------------------------------

uint32_t bmp280ReadLong(uint8_t address, uint8_t* error_code)
{
	uint32_t msb=0;
	uint32_t lsb=0;
	uint32_t xlsb=0;
	uint32_t data;
	//Send Start Condition
	i2cSendStart();	
	*error_code=i2cWaitForComplete();
	if (*error_code==0){
		*error_code=checki2cReturnCode(TW_START);
	}

	//Tell Salve go in receive mode
	if (*error_code==0){	
		i2cSendByte(BMP280_W);	// write 0xEE
		*error_code=i2cWaitForComplete();
	}

	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MT_SLA_ACK);
	}

	//Send read address to Salve
	if (*error_code==0){	
		i2cSendByte(address);	// write register address
		*error_code=i2cWaitForComplete();
	}

	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MT_DATA_ACK);
	}

	//Send Restart to Slave
	if (*error_code==0){	
		i2cSendStart();
		*error_code=i2cWaitForComplete();
	}
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_REP_START);
	}

	//Tell Salve go in send mode
	if (*error_code==0){	
		i2cSendByte(BMP280_R);	// read 0xEF
		*error_code=i2cWaitForComplete();
	}

	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MR_SLA_ACK);
	}

	//Get MSB from Salve
	if (*error_code==0){	
		i2cReceiveByteACK();
		*error_code=i2cWaitForComplete();
	}

	if (*error_code==0){	
		msb = i2cGetReceivedByte();	// Get MSB result
		*error_code=i2cWaitForComplete();
	}

	//ACKM
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MR_DATA_ACK);
	}

	//Get LSB from Slave
	if (*error_code==0){
		i2cReceiveByteNACK();
		*error_code=i2cWaitForComplete();
	}

	if (*error_code==0){	
		lsb = i2cGetReceivedByte();	// Get LSB result
		*error_code=i2cWaitForComplete();
	}

	//Get XLSB from Slave
	if (*error_code==0){
		i2cReceiveByteNACK();
		*error_code=i2cWaitForComplete();
	}

	if (*error_code==0){	
		xlsb = i2cGetReceivedByte();	// Get XLSB result
		*error_code=i2cWaitForComplete();
	}

	//NACKM
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MR_DATA_NACK);
	}

	//Send Stop on the Bus
	if (*error_code==0){
		*error_code=i2cSendStop();
	}

	data = ((msb << 16) + (lsb << 8) + xlsb) >> 4;
	return data;
}
//----------------------------------------

uint16_t bmp280WriteByte(uint8_t address, uint8_t data, uint8_t* error_code)
{
	//Send Start Condition
	i2cSendStart();	
	*error_code=i2cWaitForComplete();

	if (*error_code==0){
		*error_code=checki2cReturnCode(TW_START);
	}

	//Tell Salve go in receive mode
	if (*error_code==0){	
		i2cSendByte(BMP280_W);	// write 0xEE
		*error_code=i2cWaitForComplete();
	}
	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MT_SLA_ACK);
	}

	//Write control register address
	if (*error_code==0){	
		i2cSendByte(address);	// Register Address
		*error_code=i2cWaitForComplete();
	}
	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MT_DATA_ACK);
	}

	//Write control register value
	if (*error_code==0){	
		i2cSendByte(data);	// Data
		*error_code=i2cWaitForComplete();
	}
	//ACKS
	if (*error_code==0){	
		*error_code=checki2cReturnCode(TW_MT_DATA_ACK);
	}

	//Send Stop on the Bus
	if (*error_code==0){
		*error_code=i2cSendStop();
	}
}
//----------------------------------------

int32_t bmp280ReadTemp(uint8_t* error_code)
{
	int32_t temperature = 0;
	
	if (*error_code==0){
		temperature = bmp280ReadLong(BMP280_REG_RESULT_TEMPRERATURE,error_code);
	}	
	return temperature;
}
//----------------------------------------

int32_t bmp280ReadPressure(uint8_t* error_code)
{
	int32_t pressure = 0;

	if (*error_code==0){
		pressure = bmp280ReadLong(BMP280_REG_RESULT_PRESSURE,error_code);
	}
	
	return pressure;
}
//----------------------------------------

void bmp280Convert(int32_t* temperature, int32_t* pressure, uint8_t* error_code)
{
	int32_t adc_T;
	int32_t adc_P;

	adc_T = bmp280ReadTemp(error_code);
	adc_P = bmp280ReadPressure(error_code);

	double var1, var2, T, p, t_fine;
	var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
	t_fine = (var1 + var2);
	*temperature = (int32_t) (t_fine*10 / 5120.0);

	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
	if (var1 == 0.0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)dig_P8) / 32768.0;
	p = (p + (var1 + var2 + ((double)dig_P7)) / 16.0);

	*pressure = p;
}
//----------------------------------------

void BMP280_Begin(uint8_t* error_code)
{

	if (*error_code==0){	
		dig_T1 = bmp280ReadShort(0x88,error_code);//dig_T1
	}
	if (*error_code==0){	
		 dig_T2 = bmp280ReadShort(0x8A,error_code);//dig_T2
	}
	if (*error_code==0){	
		 dig_T3 = bmp280ReadShort(0x8C,error_code);//dig_T3
	}
	if (*error_code==0){	
		 dig_P1 = bmp280ReadShort(0x8E,error_code);//dig_P1
	}
	if (*error_code==0){	
		 dig_P2 = bmp280ReadShort(0x90,error_code);//dig_P2
	}
	if (*error_code==0){	
		 dig_P3 = bmp280ReadShort(0x92,error_code);//dig_P3
	}
	if (*error_code==0){	
		 dig_P4 = bmp280ReadShort(0x94,error_code);//dig_P4
	}
	if (*error_code==0){	
		 dig_P5 = bmp280ReadShort(0x96,error_code);//dig_P5
	}
	if (*error_code==0){	
		 dig_P6 = bmp280ReadShort(0x98,error_code);//dig_P6
	}
	if (*error_code==0){	
		 dig_P7 = bmp280ReadShort(0x9A,error_code);//dig_P7
	}
	if (*error_code==0){	
		 dig_P8 = bmp280ReadShort(0x9C,error_code);//dig_P8
	}
	if (*error_code==0){	
		 dig_P9 = bmp280ReadShort(0x9E,error_code);//dig_P9
	}

	bmp280WriteByte(BMP280_REG_CONFIG, BMP280_CONFIG, error_code);
	bmp280WriteByte(BMP280_REG_CONTROL, BMP280_MEAS, error_code);
}

//----------------------------------------

int32_t bmp280CalcAltitude(int32_t pressure){
	float temp;
	int32_t altitude;
	temp = (float) pressure/101325;
	temp = 1-pow(temp, 0.19029);
	altitude = 44330*temp*100;
	//get altitude in dm
	return altitude;
}
