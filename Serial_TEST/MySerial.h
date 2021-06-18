#ifndef MYSERIAL_H_
	#define MYSERIAL_H_
	void Serial_Begin(void);
	uint8_t Serial_Read(void);
	uint8_t Serial_Available(void);
	void Serial_Write_char(uint8_t data);
	void Serial_Write_AsCharArray(uint8_t data);
	void Serial_Write_NewLine(void);
#endif /* MYSERIAL_H_ */