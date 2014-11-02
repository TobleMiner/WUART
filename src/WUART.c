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

#include "NRF24L01/NRF24L01.h"
#include "UART/uart.h"

struct
{
	uint8_t sending:1;
	uint8_t data_ready:1;
} NRF;

uint8_t uart_timeout = 0;
uint8_t uart_threshold = 12;

int main(void)
{
	NRF.sending = FALSE;
	NRF.data_ready = FALSE;
	TCCR0B = (1<<CS02); //timer0: Prescaler = 256 => ~122 Hz
	TIMSK0 = (1<<TOIE0); //timer0: Enable overflow interrupt
	EICRA = (1<<ISC01);	//Set external interrupt on falling edge for INT0
	EIMSK = (1<<INT0);	//Enable INT0
	uart_init();
	uart_init_rx();
	uart_init_tx();
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
	uint8_t paddr[5] = ADDR_P;
	#if DLEVEL >= 2
		uart_write_async("ADDRs: ");
		uart_send_async(myaddr, 0, 5);
		uart_write_async(" ");
		uart_send_async(paddr, 0, 5);
		uart_write_async("\n");
	#endif
	set_addrs(myaddr, paddr, 5);
    while(1)
    {
        set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_enable();
		sleep_cpu();
		if(!NRF.sending && uart_data_available() && (uart_data_available() >= WIRELESS_PACK_LEN || uart_timeout >= uart_threshold))
		{
			uint8_t* data = malloc(32);
			uint8_t* dataptr = data + 1;
			#if DLEVEL >= 2
				uart_write_async("Trying uart data read...\n");
			#endif
			uint8_t len = uart_read(dataptr, 31);
			#if DLEVEL >= 2
				char* str = malloc(20);
				sprintf(str, "Sending %u bytes\n", len);
				uart_write_async(str);
				free(str);
			#endif
			*data = len;
			NRF24L01_send_data(data, 32);
			NRF.sending = TRUE;
			free(data);
		}
		if(NRF.data_ready)
		{
			uint8_t* data = malloc(32);
			NRF24L01_get_received_data(data, 32);
			NRF.data_ready = FALSE;
			#if DLEVEL >= 2
				char* str = malloc(20);
				sprintf(str, "Received %u bytes\n", *data);
				uart_write_async(str);
				free(str);
			#endif
			uart_send_async(data, 1, *data);
			free(data); 
		}
	}
}

void set_addrs(uint8_t* myaddr, uint8_t* paddr, uint8_t len)
{
	NRF24L01_set_tx_addr(paddr, len);
	NRF24L01_set_rx_addr(0, paddr, len);
	NRF24L01_set_rx_addr(1, myaddr, len);
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
	uart_irq_rx();
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