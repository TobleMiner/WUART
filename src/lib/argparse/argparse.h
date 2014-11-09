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

#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__
	
	#define ARGS_DLEVEL 0
		
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	
	#if ARGS_DLEVEL > 0
		#include "../UART/uart.h" //Assuming both libs are in the same parent folder
	#endif
	
	extern uint8_t	args_split(char* str, char*** args);
	extern void		args_free(char** args, uint8_t argnum);
	extern void		args_hex_str_array_to_byte_arrays_8bit(char** strs, uint8_t offset, uint8_t strnum, uint8_t* data);
	
	#include "argparse.c"
#endif