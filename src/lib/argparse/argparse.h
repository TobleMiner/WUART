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