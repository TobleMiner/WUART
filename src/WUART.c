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

#include "WUART.h"
#include "config/config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/NRF24L01/NRF24L01.h"
#include "lib/UART/uart.h"
#if ENABLE_CMD_MODE
	#include "lib/argparse/argparse.h"
#endif

struct
{
	uint8_t sending:1;
	uint8_t data_ready:1;
	#if ENABLE_CMD_MODE
		uint8_t cmd_mode:1;
		uint8_t lf_detect:1;
	#endif
} NRF;

uint8_t uart_timeout = 0;
uint8_t uart_threshold = 12;

int main(void)
{
	NRF.sending = FALSE;
	NRF.data_ready = FALSE;
	#if ENABLE_CMD_MODE
		NRF.cmd_mode = FALSE;
	#endif
	TCCR0B = (1<<CS02); //timer0: Prescaler = 256 => ~122 Hz
	TIMSK0 = (1<<TOIE0); //timer0: Enable overflow interrupt
	EICRA = (1<<ISC01);	//Set external interrupt on falling edge for INT0
	EIMSK = (1<<INT0);	//Enable INT0
	#if ENABLE_CMD_MODE
		EICRA |= (1<<ISC10);
		EIMSK |= (1<<INT1);
		NRF.cmd_mode = (PIND & (1<<PIND3)) > 0;
		#if DLEVEL >= 2
			if(NRF.cmd_mode)
				uart_write_async("Entering command mode\n");
			else
				uart_write_async("Leaving command mode\n");
		#endif
	#endif
	uart_init_tx();
	uart_init();
 	sei();
	NRF24L01_init();
	#if DLEVEL >= 2
		uart_write_async("NRF initialized\n");
	#endif
	_delay_ms(200);
	#if DLEVEL >= 2
		char* strbuff = malloc(64);
		nrf24l01_rf_ch_t* rf_ch = malloc(sizeof(nrf24l01_rf_ch_t));
		rf_ch->value = NRF24L01_LOW_get_register(NRF24L01_REG_RF_CH);
		nrf24l01_rf_setup_t* rf_setup = malloc(sizeof(nrf24l01_rf_setup_t));
		rf_setup->value = NRF24L01_LOW_get_register(NRF24L01_REG_RF_SETUP);
		sprintf(strbuff, "Channel:%u\nSpeed:%u\nPower:%u\n", rf_ch->rf_ch, rf_setup->rf_dr, rf_setup->rf_pwr);
		free(rf_ch);
		free(rf_setup);
		uart_write_async(strbuff);
		free(strbuff);
	#endif
	NRF24L01_set_rx();
	uint8_t myaddr[5] =  ADDR_ME;
	#ifdef ADDR_P
		uint8_t paddr[5] = ADDR_P;
	#endif
	#if DLEVEL >= 2
		uart_write_async("ADDRs: ");
		uart_send_async(myaddr, 0, 5);
		#ifdef ADDR_P
			uart_write_async(" ");
			uart_send_async(paddr, 0, 5);
		#endif
		uart_write_async("\n");
	#endif
	set_addr(myaddr, 5);
	#ifdef ADDR_P
		set_addr_p(paddr, 5);
	#endif
	#if ENABLE_CMD_MODE && DLEVEL >= 2
		if(NRF.cmd_mode)
			uart_write_async("Entering command mode\n");
	#endif
	uart_flush_rx(); //Ready, initialize UART to defined state
	uart_init_rx();
	while(1)
    {
        set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_enable();
		sleep_cpu();
		#if ENABLE_CMD_MODE
			if(NRF.cmd_mode)
			{
				if(uart_data_available() >= UART_BUFF_LEN_RX)
				{
					uart_flush_rx(); //Unstuck device
					continue;
				}
				if(!NRF.lf_detect) //LF
				{
					continue;
				}
				NRF.lf_detect = FALSE;
				#if DLEVEL >= 2
					uart_write_async("<LF> detected\n");
				#endif
				uint32_t data_len = uart_data_available();
				char* str = malloc(data_len + 1);
				if(str == NULL)
				{
					#if DLEVEL >= 1
						uart_write_async("OUT OF MEM\n");
						continue;
					#endif
				}
				uart_read_line(str);
				uart_flush_rx();
				char*** args = malloc(sizeof(char***));
				uint8_t argnum = args_split(str, args);
				free(str);
				if(process_cmd(*args, argnum))
				{
					uart_write_async("ERROR\n");
				}
				else
				{
					uart_write_async("OK\n");
				}
				args_free(*args, argnum);
				free(args);
				continue;
			}
			else
			{
		#endif
				if(!NRF.sending && uart_data_available() && (uart_data_available() >= WIRELESS_PACK_LEN || uart_timeout >= uart_threshold))
				{
					uint8_t* data = malloc(WIRELESS_PACK_LEN);
					uint8_t* dataptr = data + 1;
					uint8_t len = uart_read(dataptr, WIRELESS_PACK_LEN - 1);
					#if DLEVEL >= 2
						char* str = malloc(20);
						sprintf(str, "Sending %u bytes\n", len);
						uart_write_async(str);
						free(str);
					#endif
					*data = len;
					NRF24L01_send_data(data, WIRELESS_PACK_LEN);
					NRF.sending = TRUE;
					free(data);
				}
				if(NRF.data_ready)
				{
					uint8_t* data = malloc(WIRELESS_PACK_LEN);
					NRF24L01_get_received_data(data, WIRELESS_PACK_LEN);
					NRF.data_ready = FALSE;
					#if DLEVEL >= 2
						char* str = malloc(20);
						sprintf(str, "Received %u bytes\n", *data);
						uart_write_async(str);
						free(str);
					#endif
					if(*data > WIRELESS_PACK_LEN - 1)
					*data = WIRELESS_PACK_LEN - 1;
					uart_send_async(data, 1, *data);
					free(data);
				}
		#if ENABLE_CMD_MODE
			}
		#endif
	}
}

#if ENABLE_CMD_MODE
	uint8_t process_cmd(char** args, uint8_t argnum)
	{
		if(argnum == 0)
			return PROCESS_RESULT_NO_CMD;
		char* cmd = args[0];
		argnum--;
		#if DLEVEL >= 2
			uart_write_async("Command: ");
			uart_write_async(cmd);
			uart_write_async("\n");
		#endif
		if(strcmp(cmd, "AT") == 0)
		{
			return PROCESS_RESULT_OK;
		}
		if(strcmp(cmd, "ATA") == 0)
		{
			if(argnum < 5)
				return PROCESS_RESULT_TOO_FEW_ARGS;
			uint8_t* addr = malloc(5);
			args_hex_str_array_to_byte_arrays_8bit(args, 1, 5, addr);
			set_addr(addr, 5);
			free(addr);
			return PROCESS_RESULT_OK;
		}
		if(strcmp(cmd, "ATR") == 0)
		{
			if(argnum < 5)
				return PROCESS_RESULT_TOO_FEW_ARGS;
			uint8_t* addr = malloc(5);
			args_hex_str_array_to_byte_arrays_8bit(args, 1, 5, addr);
			set_addr_p(addr, 5);
			free(addr);
			return PROCESS_RESULT_OK;
		}
		if(strcmp(cmd, "ATB") == 0)
		{
			if(argnum < 1)
				return PROCESS_RESULT_TOO_FEW_ARGS;
			uint32_t baudrate = (uint32_t)strtol(args[1], NULL, 0);
			uart_set_baudrate(baudrate);
			return PROCESS_RESULT_OK;
		}
		if(strcmp(cmd, "ATC") == 0)
		{
			if(argnum < 1)
				return PROCESS_RESULT_TOO_FEW_ARGS;
			uint8_t channel = (uint8_t)strtol(args[1], NULL, 0);
			if(channel > 125)
				return PROCESS_RESULT_ARG_OUT_OF_RANGE;
			NRF24L01_set_channel(channel);
			return PROCESS_RESULT_OK;
		}
		if(strcmp(cmd, "ATP") == 0)
		{
			if(argnum < 1)
				return PROCESS_RESULT_TOO_FEW_ARGS;
			uint8_t tx_pwr = (uint8_t)strtol(args[1], NULL, 0);
			if(tx_pwr > 3)
				return PROCESS_RESULT_ARG_OUT_OF_RANGE;
			NRF24L01_set_tx_pwr(tx_pwr);
			return PROCESS_RESULT_OK;
		}
		if(strcmp(cmd, "ATD") == 0)
		{
			if(argnum < 1)
				return PROCESS_RESULT_TOO_FEW_ARGS;
			uint8_t rf_dr = (uint8_t)strtol(args[1], NULL, 0);
			if(rf_dr > 2)
				return PROCESS_RESULT_ARG_OUT_OF_RANGE;
			NRF24L01_set_rf_dr(rf_dr);
			return PROCESS_RESULT_OK;
		}
		return PROCESS_RESULT_UNKNOWN_COMMAND;
	}
#endif

void set_addr(uint8_t* myaddr, uint8_t len)
{
	NRF24L01_set_rx_addr(1, myaddr, len);
}

void set_addr_p(uint8_t* paddr, uint8_t len)
{
	NRF24L01_set_tx_addr(paddr, len);
	NRF24L01_set_rx_addr(0, paddr, len);	
}

ISR(TIMER0_OVF_vect)
{
	if(uart_timeout <= uart_threshold)
		uart_timeout++;
	#if DLEVEL >= 2
		if(uart_timeout == uart_threshold)
		{
			char* str = malloc(30);
			sprintf(str, "IRQ: %u bytes stored\n", (uint16_t)uart_data_available());
			uart_write_async(str);
			free(str);
		}
	#endif
}

ISR(USART_RX_vect)
{
	uart_timeout = 0;
	#if ENABLE_CMD_MODE
		uint8_t udr = UDR0;
		if(NRF.cmd_mode && udr == 0x0A)
			NRF.lf_detect = TRUE;
		uart_irq_rx(udr);
	#else
		uart_irq_rx(UDR0);
	#endif
}

ISR(INT0_vect)
{
	uint8_t status = NRF24L01_get_status();
	if(status & NRF24L01_MASK_STATUS_TX_DS)
	{
		#if DLEVEL >= 2
			uart_write_async("IRQ: Data sent\n");
		#endif
		NRF24L01_set_rx();
		NRF.sending = FALSE;
	}
	if(status & NRF24L01_MASK_STATUS_RX_DR)
	{
		#if DLEVEL >= 2
			uart_write_async("IRQ: Data ready\n");
		#endif
		NRF.data_ready = TRUE;
	}
	NRF24L01_LOW_set_register(NRF24L01_REG_STATUS, status & (NRF24L01_MASK_STATUS_RX_DR | NRF24L01_MASK_STATUS_TX_DS | NRF24L01_MASK_STATUS_MAX_RT));
	if(status & NRF24L01_MASK_STATUS_MAX_RT)
	{
		NRF24L01_CE_LOW;
		_delay_us(10);
		NRF24L01_CE_HIGH;
	}
}

#if ENABLE_CMD_MODE
	ISR(INT1_vect)
	{
		NRF.cmd_mode = (PIND & (1<<PIND3)) > 0;
		#if DLEVEL >= 2
			if(NRF.cmd_mode)
				uart_write_async("Entering command mode\n");
			else
				uart_write_async("Leaving command mode\n");
		#endif
		uart_flush_rx();
	}
#endif