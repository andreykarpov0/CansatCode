#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include "MyTWI.h"

uint8_t twi(uint8_t act){
	switch(act){
		case TWI_START:
		case TWI_RESTART:
		TWCR = 1 << (TWSTA) | 1 << (TWEN) | 1 << (TWINT);// ���� ����� ���������� | 1 << (TWIE);
		break;
		case TWI_STOP:
		TWCR = 1 << (TWSTO) | 1 << (TWEN) | 1 << (TWINT);// | 1 << (TWIE);
		break;
		case TWI_TRANSMIT:
		TWCR = 1 << (TWEN) | 1 << (TWINT);// | 1 << (TWIE);
		break;
		case TWI_RECEIVE_ACK:
		TWCR|=(1<<TWEA);
		TWCR|=(1<<TWINT);
		break;
		case TWI_RECEIVE_NACK:
		TWCR&=(~(1<<TWEA));
		TWCR|=(1<<TWINT);
		break;
	}
	if(act != TWI_STOP)while (!(TWCR & 1 << (TWINT)));
	return (TWSR & 0xF8);
}

void TWI_Init(unsigned long int CPU_F, unsigned long int SCL_F){
	TWBR = ( ( (CPU_F) / (SCL_F) - 16) / 2);
	TWSR = 0;
	TWCR |= (1 << TWEN);
}
void TWI_Close(){
	TWCR &= ~(1 << TWEN);
}
void TWI_Start(){
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTA);
	while(!(TWCR & (1 << TWINT)));
}
void TWI_Stop(){
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);
	while(TWCR & (1 << TWSTO));
}
uint8_t TWI_WriteByte(uint8_t data){
	TWDR = data;
	TWCR = (1 << TWEN)|(1 << TWINT);
	while(!(TWCR & (1 << TWINT)));
	
	if((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28 ||(TWSR & 0xF8) == 0x40) { return true; } //if received
	else { return false; } //if error
}
uint8_t TWI_ReadByte(uint8_t *data, uint8_t ack){
	if(ack)	{ TWCR |= (1 << TWEA); } //enable ack
	else	{ TWCR &= ~(1 << TWEA);} //without ack
	
	TWCR |= (1 << TWINT);
	while(!(TWCR & (1 << TWINT)));
	
	if((TWSR & 0xF8) == 0x58 || (TWSR & 0xF8) == 0x50){ *data = TWDR; return true; } //if data received
	else { return false; }
}
void TWI_WriteRegister(uint8_t addr, uint8_t reg, uint8_t val){
	TWI_Start();
	TWI_WriteByte(addr << 1);
	TWI_WriteByte(reg);
	TWI_WriteByte(val);
	_delay_ms(10);
	TWI_Stop();
	_delay_ms(6);
}
void TWI_ReadBytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t num, uint8_t *buffer){
	TWI_Start();
	TWI_WriteByte(dev_addr << 1);
	TWI_WriteByte(reg_addr);
	
	TWI_Start();
	TWI_WriteByte((dev_addr << 1) + 1);
	
	for(uint8_t i = 0; i < num; ++i){
		TWI_ReadByte(buffer, 1);
		++buffer;
	}
}