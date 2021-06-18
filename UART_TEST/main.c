/*
 * UART_TEST.c
 *
 * Created: 09.06.2021 19:36:35
 * Author : akarp
 */ 

#include <avr/io.h>

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS)|(3<<UCSZ0);
}

int main(void)
{

	USART_Init ( MYUBRR );
	unsigned char x;
    /* Replace with your application code */
    while (1) 
    {
		x = USART_Receive();
		USART_Transmit(x);
    }
}

