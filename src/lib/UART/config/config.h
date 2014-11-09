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

#ifndef __UART_LIB_CONF__
#define __UART_LIB_CONF__
	#define UART_BAUDRATE				19200
	#define UART_ENABLE_RX				TRUE
	#define UART_BUFF_LEN_RX			256
	#define UART_ENABLE_TX				TRUE
	#define UART_BUFF_LEN_TX			256
	
	#define UART_IRQ_HOOK_RX			TRUE
	#define UART_IRQ_HOOK_UDRE			FALSE
	#define UART_IRQ_HOOK_TX			FALSE
	
	#define UART_ENABLE_FLOWCONTROL		TRUE
	#define UART_CTS_DDR				DDRD
	#define UART_CTS_PORT				PORTD
	#define UART_CTS_PIN				PIND4
	#define UART_FLOWCONTROL_BUFF_FILL	128
#endif