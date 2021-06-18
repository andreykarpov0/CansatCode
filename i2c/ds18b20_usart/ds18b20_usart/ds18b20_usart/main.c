#define  F_CPU 8000000
#define  FOSC 8000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void USART_INIT()
{
	DDRD = 0b10000000;
	UBRR0H = 0;
	UBRR0L = 51;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	PORTD = (1<<PD7);
	_delay_ms(1000);
}
int USART_RECIVE()
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
void USART_TRANSMITT(uint8_t data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}
void STRING_TRANSMITT(char string[])
{
	for(int i = 0;string[i] != 0;i++)
	{
		USART_TRANSMITT((uint8_t)string[i]);
	}
}
int main()
{
	USART_INIT();
	PORTD = (0<<PD7);
	char str[] = "Output: ";
	while(1)
	{
		int data = USART_RECIVE();
		if(data=='1')
		{
			PORTD = (1<<PD7);
		}
		else
		{
			PORTD = (0<<PD7);
		}
		USART_TRANSMITT(0x0d);
		USART_TRANSMITT(0x0a);
		STRING_TRANSMITT(str);
		USART_TRANSMITT(data);
	}
}