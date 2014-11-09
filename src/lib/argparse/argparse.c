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

extern uint8_t args_split(char* str, char*** args)
{
	char* allocated = NULL;
	if(*(str + strlen(str) - 1) != 0x20)
	{
		char *tstr = malloc(strlen(str) + 2); //Add one additional space and reserve mem for \0 byte
		if(tstr == NULL)
		{
			#if ARGS_DLEVEL >= 1
				uart_write_async("ARGSPLIT: No mem to append space\n");
			#endif
			return 0; //Failed to alloc mem
		}
		allocated = tstr;
		strcpy(tstr, str);
		str = strcat(tstr, " ");
	}
	char** argptr = NULL;
	uint8_t argcnt = 0;
	char* ptr = strpbrk(str, " ");
	char* ptr_last = str;
	while(ptr != NULL)
	{
		argptr = realloc(argptr, sizeof(char*) * (argcnt + 1));
		if(argptr == NULL)
		{
			free(allocated);
			#if ARGS_DLEVEL >= 1
				uart_write_async("ARGSPLIT: Failed to realloc arg buff\n");
			#endif
			return 0; //Failed to realloc mem
		}
		char** targptr = argptr + argcnt;
		char* arg = malloc(ptr - ptr_last + 1);
		if(arg == NULL)
		{
			free(allocated);
			#if ARGS_DLEVEL >= 1
				uart_write_async("ARGSPLIT: Failed to alloc str buff\n");
			#endif
			return 0; //Failed to alloc mem
		}
		strncpy(arg, ptr_last, ptr - ptr_last);
		*(arg + (ptr - ptr_last)) = 0;
		*targptr = arg;
		argcnt++;
		str = ptr + 1;
		ptr_last = str;
		ptr = strpbrk(str, " ");
	}
	*args = argptr;
	#if ARGS_DLEVEL >= 2
		char* strbuff = malloc(16);
		sprintf(strbuff, "%d arguments\n", argcnt);
		uart_write_async(strbuff);
		free(strbuff);
		uart_write_async("Arguments:\n");
		for(uint8_t i = 0; i < argcnt; i++)
		{
			uart_write_async(argptr[i]);
			uart_write_async("\n");
		}
	#endif
	free(allocated);
	return argcnt;
}

extern void args_free(char** args, uint8_t argnum)
{
	for(uint8_t i = 0; i < argnum; i++)
	{
		free(args[i]);
	}
	free(args);
}

extern void args_hex_str_array_to_byte_arrays_8bit(char** strs, uint8_t offset, uint8_t strnum, uint8_t* data)
{
	char** strptr = strs + offset;
	uint8_t* dataptr = data;
	for(uint8_t i = 0; i < strnum; i++)
	{
		*dataptr = (uint8_t)strtol(*strptr, NULL, 16);
		strptr++;
		dataptr++;
	}
}