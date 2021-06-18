//#################################################################################
//#################################################################################
//#################################################################################
/*	Library to use UART with ATMEL Atmega family
	
	Copyright (C) 2010 Stefan Sicklinger

	For support check out http://www.sicklinger.com
    
	This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
//#################################################################################
//#################################################################################
//#################################################################################
#include "uart.h"

void uart_init(){
	//Compute transfer rate for 8 MHz clock frequency
	uint16_t tmp=0;
	tmp = ((uint32_t)F_CPU/(BAUDRATE*16.0)-1.0);
	UBRRL =(uint8_t)tmp;
	UBRRH =(uint8_t)(tmp >> 8);
	// Enable send & receive & receive interrupt
	UCSRB |= (1<<TXEN)|(1<<RXEN);
	// 8 Bit mode no parity no stopbit 
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	stdout = &mystdout;      // Detour
}



void send_char_uart(unsigned char c)
{
    while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
    {
    }                             
 
    UDR = c;                      /* sende Zeichen */
    
}


void send_string_uart (char *s)
{
    while (*s)
    {   /* so lange *s != '\0' also ungleich dem "String-Endezeichen" */
        send_char_uart(*s);
        s++;
    }
}
int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') uart_putchar('\r', stream);
  
    while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
    {
    } 
    UDR = c;
    
    return 0;
}



