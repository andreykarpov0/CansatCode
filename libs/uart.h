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
#ifndef _UART_H_
#define _UART_H_ 1
#include <avr/io.h>
#include <util/delay.h> 
#include <stdio.h>
#include <avr/interrupt.h>


//----------------------------------------
// Set BAUDRATE
//----------------------------------------
#define BAUDRATE 9600UL
//----------------------------------------
// Prototypes
//----------------------------------------
void uart_init(void);
void send_char_uart(unsigned char c);
void send_string_uart (char *s);
int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

#endif



