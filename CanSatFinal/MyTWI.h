#ifndef MYTWI_H_
	#define MYTWI_H_
	#define TWI_START			0
	#define TWI_RESTART         1
	#define TWI_STOP            2
	#define TWI_TRANSMIT        3
	#define TWI_RECEIVE_ACK     4
	#define TWI_RECEIVE_NACK    5
	
	void TWI_Init(unsigned long int CPU_F, unsigned long int SCL_F);
	void TWI_Close();
	void TWI_Start();
	void TWI_Stop();
	
	uint8_t TWI_WriteByte(uint8_t data);
	uint8_t TWI_ReadByte(uint8_t *data, uint8_t ack);
	
	void TWI_WriteRegister(uint8_t addr, uint8_t reg, uint8_t val);
	void TWI_ReadBytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t num, uint8_t *buffer);
#endif /* MYTWI_H_ */