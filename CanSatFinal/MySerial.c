#include <avr/io.h>
#include <stdint.h>
#include "MySerial.h"

void Serial_Begin(void) {
	UBRR0H = 0;
	UBRR0L = 51;					/* 9600 BOD 8MHz */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);	/* Enable receiver and transmitter */
	UCSR0C = (3<<UCSZ00);			/* Set frame format: 8data, 1stop bit */
}
uint8_t Serial_Available(void) {
	if ( UCSR0A & (1<<RXC0) ) { return 1; }
	return 0;
}
uint8_t Serial_Read(void) {
	int i = 0;
	while ( !(UCSR0A & (1<<RXC0)) ) /* Wait for data to be received */
		;
	return UDR0;					/* Get and return received data from buffer */
}
void Serial_Write_char(uint8_t data) {
	while ( !( UCSR0A & (1<<UDRE0)) ) /* Wait for empty transmit buffer */
	;
	UDR0 = data; /* Put data into buffer, sends the data */
}
void Serial_Write_AsCharArray(uint8_t data) {
	uint8_t f, temp, config = 0;
	temp = data / 100; f = data % 10;
	if (temp != 0) { Serial_Write_char(0b00110000 | temp); config |= 0b00000010; }
	temp = ((data - temp * 100) - f) / 10;
	if ((temp != 0) || (config & 0b00000010)) { Serial_Write_char(0b00110000 | temp); }
	Serial_Write_char(0b00110000 | f);
}
void Serial_Write_NewLine(void) { Serial_Write_char(13); Serial_Write_char(10); }