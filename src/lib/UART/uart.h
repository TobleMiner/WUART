/*
Copyright (c) 2014 Tobias Schramm

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef __UART_LIB_H__
#define __UART_LIB_H__

	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif

	//Config
	#include "config/config.h"
	
	//Libs
	#include <stdint.h>
		
	//Functions
	extern void		uart_init(void);
	extern void		uart_set_baudrate(uint32_t baudrate);
	#if UART_ENABLE_TX == TRUE
	extern void		uart_init_tx(void);
	extern void		uart_send_byte(uint8_t byte);
	extern void		uart_send_async(uint8_t* data, uint16_t offset, uint16_t len);
	extern void		uart_send_sync(uint8_t* data, uint16_t offset, uint16_t len);
	extern void		uart_write_sync(char* str);
	extern void		uart_write_async(char* str);
	extern void		uart_irq_tx(void);
	extern void		uart_irq_udre(void);
	#endif
	
	#if UART_ENABLE_RX == TRUE
	extern void		uart_irq_rx(uint8_t udr);
	extern void		uart_init_rx(void);
	extern uint32_t	uart_data_available(void);
	extern uint32_t	uart_read(uint8_t* data, uint32_t len);
	extern uint32_t	uart_read_line(char* str);
	extern void		uart_flush_rx(void);
	#endif
	
	#include "uart.c"
#endif
