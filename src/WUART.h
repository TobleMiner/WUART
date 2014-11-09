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

#ifndef __WUART_H__
#define __WUART_H__

	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif

	#include <stdint.h>
	
	int		main(void);
	void	set_addr(uint8_t* myaddr, uint8_t len);
	void	set_addr_p(uint8_t* paddr, uint8_t len);
	uint8_t	process_cmd(char** args, uint8_t argnum);
	
	#define PROCESS_RESULT_OK				0
	#define PROCESS_RESULT_OUT_OF_MEM		1
	#define PROCESS_RESULT_NO_CMD			2
	#define PROCESS_RESULT_UNKNOWN_COMMAND	3
	#define PROCESS_RESULT_TOO_FEW_ARGS		4
	#define PROCESS_RESULT_ARG_OUT_OF_RANGE	5
#endif