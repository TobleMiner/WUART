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

#ifndef __NRF24L01_LIB_H__
	#define __NRF24L01_LIB_H__
	
	//Typedefs
	typedef union
	{
		struct
		{
			uint8_t tx_full:1;
			uint8_t rx_p_no:3;
			uint8_t max_rt:1;
			uint8_t tx_ds:1;
			uint8_t rx_dr:1;
			uint8_t reserved:1;
		};
		uint8_t value;
	}
	nrf24l01_status_t;
	
	typedef union
	{
		struct
		{
			uint8_t prim_rx:1;
			uint8_t pwr_up:1;
			uint8_t crco:1;
			uint8_t en_crc:1;
			uint8_t mask_max_rt:1;
			uint8_t mask_tx_ds:1;
			uint8_t mask_rx_dr:1;
			uint8_t reserved:1;
		};
		uint8_t value;
	}
	nrf24l01_config_t;
	
	typedef union
	{
		struct
		{
			uint8_t enaa_p0:1;
			uint8_t enaa_p1:1;
			uint8_t enaa_p2:1;
			uint8_t enaa_p3:1;
			uint8_t enaa_p4:1;
			uint8_t enaa_p5:1;
			uint8_t reserved:2;
		};
		uint8_t value;
	}
	nrf24l01_shockburst_t;

	typedef union
	{
		struct
		{
			uint8_t erx_p0:1;
			uint8_t erx_p1:1;
			uint8_t erx_p2:1;
			uint8_t erx_p3:1;
			uint8_t erx_p4:1;
			uint8_t erx_p5:1;
			uint8_t reserved:2;
		};
		uint8_t value;
	}
	nrf24l01_en_rxaddr_t;
	
	typedef union
	{
		struct
		{
			uint8_t aw:2;
			uint8_t reserved:6;
		};
		uint8_t value;
	}
	nrf24l01_setup_aw_t;

	typedef union
	{
		struct
		{
			uint8_t arc:4;
			uint8_t ard:4;
		};
		uint8_t value;
	}
	nrf24l01_setup_retr_t;

	typedef union
	{
		struct
		{
			uint8_t rf_ch:7;
			uint8_t reserved:1;
		};
		uint8_t value;
	}
	nrf24l01_rf_ch_t;

	typedef union
	{
		struct
		{
			uint8_t lna_hcurr:1;
			uint8_t rf_pwr:2;
			uint8_t rf_dr_high:1;
			uint8_t pll_lock:1;
			uint8_t rf_dr_low:1;
			uint8_t reserved:1;
			uint8_t cont_wave:1;
		};
		uint8_t value;
	}
	nrf24l01_rf_setup_t;	

	typedef union
	{
		struct
		{
			uint8_t arc_cnt:4;
			uint8_t plos_cnt:4;
		};
		uint8_t value;
	}
	nrf24l01_observe_tx_t;
	
	typedef union
	{
		struct
		{
			uint8_t cd:1;
			uint8_t reserved:7;
		};
		uint8_t value;
	}
	nrf24l01_cd_t;

	typedef union
	{
		struct
		{
			uint8_t rx_pw:6;
			uint8_t reserved:2;
		};
		uint8_t value;
	}
	nrf24l01_rx_pw_t;

	typedef union
	{
		struct
		{
			uint8_t rx_empty:1;
			uint8_t rx_full:1;
			uint8_t reserved:2;
			uint8_t tx_empty:1;
			uint8_t tx_full:1;
			uint8_t tx_reuse:1;
			uint8_t reserved1:1;
		};
		uint8_t value;
	}
	nrf24l01_fifo_status_t;

	typedef union
	{
		struct
		{
			uint8_t dpl_p0:1;
			uint8_t dpl_p1:1;
			uint8_t dpl_p2:1;
			uint8_t dpl_p3:1;
			uint8_t dpl_p4:1;
			uint8_t dpl_p5:1;
			uint8_t reserved:2;
		};
		uint8_t value;
	}
	nrf24l01_dynpd_t;

	typedef union
	{
		struct
		{
			uint8_t en_dyn_ack:1;
			uint8_t en_ack_pay:1;
			uint8_t en_dpl:1;
			uint8_t reserved:5;
		};
		uint8_t value;
	}
	nrf24l01_feature_t;

	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif
	
	//SPI lib
	#include "spi.h"
	
	//Config files
	#include "config/io.h"
	#include "config/wireless.h"
	#include "config/configtest.h"
	
	//NRF24L01 constants
	//Registers
	#define NRF24L01_REG_CONFIG					0x00
	#define NRF24L01_REG_EN_AA					0x01
	#define NRF24L01_REG_EN_RXADDR				0x02
	#define NRF24L01_REG_SETUP_AW				0x03
	#define NRF24L01_REG_SETUP_RETR				0x04
	#define NRF24L01_REG_RF_CH					0x05
	#define NRF24L01_REG_RF_SETUP				0x06
	#define NRF24L01_REG_STATUS					0x07
	#define NRF24L01_REG_OBSERVE_TX				0x08
	#define NRF24L01_REG_CD						0x09
	#define NRF24L01_REG_RX_ADDR_P0				0x0A
	#define NRF24L01_REG_RX_ADDR_P1				0x0B
	#define NRF24L01_REG_RX_ADDR_P2				0x0C
	#define NRF24L01_REG_RX_ADDR_P3				0x0D
	#define NRF24L01_REG_RX_ADDR_P4				0x0E
	#define NRF24L01_REG_RX_ADDR_P5				0x0F
	#define NRF24L01_REG_TX_ADDR				0x10
	#define NRF24L01_REG_RX_PW_P0				0x11
	#define NRF24L01_REG_RX_PW_P1				0x12
	#define NRF24L01_REG_RX_PW_P2				0x13
	#define NRF24L01_REG_RX_PW_P3				0x14
	#define NRF24L01_REG_RX_PW_P4				0x15
	#define NRF24L01_REG_RX_PW_P5				0x16
	#define NRF24L01_REG_FIFO_STATUS			0x17
	#define NRF24L01_REG_DYNPD					0x1C
	#define NRF24L01_REG_FEATURE				0x1D
	
	//Bitmasks
	//Config
	#define NRF24L01_MASK_CONFIG				0b01111111
	#define NRF24L01_MASK_CONFIG_MASK_RX_DR		0b01000000
	#define NRF24L01_MASK_CONFIG_MASK_TX_DS		0b00100000
	#define NRF24L01_MASK_CONFIG_MASK_MAX_RT	0b00010000
	#define NRF24L01_MASK_CONFIG_EN_CRC			0b00001000
	#define NRF24L01_MASK_CONFIG_CRCO			0b00000100
	#define NRF24L01_MASK_CONFIG_PWR_UP			0b00000010
	#define NRF24L01_MASK_CONFIG_PRIM_RX		0b00000001
	
	//Enhanced ShockBurst
	#define NRF24L01_MASK_EN_AA					0b00111111
	#define NRF24L01_MASK_EN_AA_ENAA_P5			0b00100000
	#define NRF24L01_MASK_EN_AA_ENAA_P4			0b00010000
	#define NRF24L01_MASK_EN_AA_ENAA_P3			0b00001000
	#define NRF24L01_MASK_EN_AA_ENAA_P2			0b00000100
	#define NRF24L01_MASK_EN_AA_ENAA_P1			0b00000010
	#define NRF24L01_MASK_EN_AA_ENAA_P0			0b00000001
	
	//Enabled RX addresses
	#define NRF24L01_MASK_EN_RXADDR				0b00111111
	#define NRF24L01_MASK_EN_RXADDR_ERX_P5		0b00100000
	#define NRF24L01_MASK_EN_RXADDR_ERX_P4		0b00010000
	#define NRF24L01_MASK_EN_RXADDR_ERX_P3		0b00001000
	#define NRF24L01_MASK_EN_RXADDR_ERX_P2		0b00000100
	#define NRF24L01_MASK_EN_RXADDR_ERX_P1		0b00000010
	#define NRF24L01_MASK_EN_RXADDR_ERX_P0		0b00000001
	
	//Setup of Address width (common for all pipes)
	#define NRF24L01_MASK_SETUP_AW				0b00000011
	#define NRF24L01_MASK_SETUP_AW_AW			0b00000011
	
	//Setup of automatic retransmission
	#define NRF24L01_MASK_SETUP_RETR			0b11111111
	#define NRF24L01_MASK_SETUP_RETR_ARD		0b11110000
	#define NRF24L01_MASK_SETUP_RETR_ARC		0b00001111
	
	//RF channel
	#define NRF24L01_MASK_RF_CH					0b01111111
	#define NRF24L01_MASK_RF_CH_RF_CH			0b01111111
	
	//RF setup
	#define NRF24L01_MASK_RF_SETUP				0b00011111
	#define NRF24L01_MASK_RF_SETUP_PLL_LOCK		0b00010000
	#define NRF24L01_MASK_RF_SETUP_RF_DR		0b00001000
	#define NRF24L01_MASK_RF_SETUP_RF_PWR		0b00000110
	#define NRF24L01_MASK_RF_SETUP_LNA_HCURR	0b00000001
	
	//Status
	#define NRF24L01_MASK_STATUS				0b01111111
	#define NRF24L01_MASK_STATUS_RX_DR			0b01000000
	#define NRF24L01_MASK_STATUS_TX_DS			0b00100000
	#define NRF24L01_MASK_STATUS_MAX_RT			0b00010000
	#define NRF24L01_MASK_STATUS_RX_P_NO		0b00001110
	#define NRF24L01_MASK_STATUS_TX_FULL		0b00000001
	
	//Transmit observe register
	#define NRF24L01_MASK_OBSERVE_TX			0b11111111
	#define NRF24L01_MASK_OBSERVE_TX_PLOS_CNT	0b11110000
	#define NRF24L01_MASK_OBSERVE_TX_ARC_CNT	0b00001111
	
	//Carrier detect
	#define NRF24L01_MASK_CD					0b00000001
	#define NRF24L01_MASK_CD_CD					0b00000001
	
	//Receive address data pipe 0, 5 bytes LSB first
	#define NRF24L01_MASK_RX_ADDR_P0			0b11111111
	#define NRF24L01_MASK_RX_ADDR_P0_RX_ADDR_P0	0b11111111

	//Receive address data pipe 1, 5 bytes LSB first
	#define NRF24L01_MASK_RX_ADDR_P1			0b11111111
	#define NRF24L01_MASK_RX_ADDR_P1_RX_ADDR_P1	0b11111111
	
	//Receive address data pipe 2
	#define NRF24L01_MASK_RX_ADDR_P2			0b11111111
	#define NRF24L01_MASK_RX_ADDR_P2_RX_ADDR_P2	0b11111111

	//Receive address data pipe 3
	#define NRF24L01_MASK_RX_ADDR_P3			0b11111111
	#define NRF24L01_MASK_RX_ADDR_P3_RX_ADDR_P3	0b11111111

	//Receive address data pipe 4
	#define NRF24L01_MASK_RX_ADDR_P4			0b11111111
	#define NRF24L01_MASK_RX_ADDR_P4_RX_ADDR_P4	0b11111111

	//Receive address data pipe 5
	#define NRF24L01_MASK_RX_ADDR_P5			0b11111111
	#define NRF24L01_MASK_RX_ADDR_P5_RX_ADDR_P5	0b11111111

	//Transmit address, 5 bytes LSB first
	#define NRF24L01_MASK_TX_ADDR				0b11111111
	#define NRF24L01_MASK_TX_ADDR_TX_ADDR		0b11111111
	
	//Number of bytes in RX payload in pipe 0
	#define NRF24L01_MASK_RX_PW_P0				0b00111111
	#define NRF24L01_MASK_RX_PW_P0_RX_PW_P0		0b00111111
	
	//Number of bytes in RX payload in pipe 1
	#define NRF24L01_MASK_RX_PW_P1				0b00111111
	#define NRF24L01_MASK_RX_PW_P1_RX_PW_P1		0b00111111
	
	//Number of bytes in RX payload in pipe 2
	#define NRF24L01_MASK_RX_PW_P2				0b00111111
	#define NRF24L01_MASK_RX_PW_P2_RX_PW_P2		0b00111111
	
	//Number of bytes in RX payload in pipe 3
	#define NRF24L01_MASK_RX_PW_P3				0b00111111
	#define NRF24L01_MASK_RX_PW_P3_RX_PW_P3		0b00111111
	
	//Number of bytes in RX payload in pipe 4
	#define NRF24L01_MASK_RX_PW_P4				0b00111111
	#define NRF24L01_MASK_RX_PW_P4_RX_PW_P4		0b00111111
	
	//Number of bytes in RX payload in pipe 5
	#define NRF24L01_MASK_RX_PW_P5				0b00111111
	#define NRF24L01_MASK_RX_PW_P5_RX_PW_P5		0b00111111
	
	//FIFO status register
	#define NRF24L01_MASK_FIFO_STATUS_TX_REUSE	0b01000000
	#define NRF24L01_MASK_FIFO_STATUS_TX_FULL	0b00100000
	#define NRF24L01_MASK_FIFO_STATUS_TX_EMPTY	0b00010000
	#define NRF24L01_MASK_FIFO_STATUS_RX_FULL	0b00000010
	#define NRF24L01_MASK_FIFO_STATUS_RX_EMPTY	0b00000001
	
	//Enable dynamic payload length
	#define NRF24L01_MASK_DYNPD_DPL_P5			0b00100000
	#define NRF24L01_MASK_DYNPD_DPL_P4			0b00010000
	#define NRF24L01_MASK_DYNPD_DPL_P3			0b00001000
	#define NRF24L01_MASK_DYNPD_DPL_P2			0b00000100
	#define NRF24L01_MASK_DYNPD_DPL_P1			0b00000010
	#define NRF24L01_MASK_DYNPD_DPL_P0			0b00000001
	
	//Feature register
	#define NRF24L01_MASK_FEATURE_EN_DPL		0b00000100
	#define NRF24L01_MASK_FEATURE_EN_ACK_PAY	0b00000010
	#define NRF24L01_MASK_FEATURE_EN_DYN_ACK	0b00000001
	
	//SPI commands
	#define NRF24L01_CMD_R_REGISTER				0b00000000
	#define NRF24L01_CMD_W_REGISTER				0b00100000
	#define NRF24L01_CMD_R_RX_PAYLOAD			0b01100001
	#define NRF24L01_CMD_W_TX_PAYLOAD			0b10100000
	#define NRF24L01_CMD_FLUSH_TX				0b11100001
	#define NRF24L01_CMD_FLUSH_RX				0b11100010
	#define NRF24L01_CMD_REUSE_TX_PL			0b11100011
	#define NRF24L01_CMD_ACTIVATE				0b01010000
	#define NRF24L01_CMD_R_RX_PL_WID			0b01100000
	#define NRF24L01_CMD_W_ACK_PAYLOAD			0b10101000
	#define NRF24L01_CMD_W_TX_PAYLOAD_NO_ACK	0b10110000
	#define NRF24L01_CMD_NOP					0b11111111
		
	//Dynamically generated config
	//IO pin controls
	#define NRF24L01_CE_HIGH	MODULE_CE_PORT	|= (1<<MODULE_CE_PIN)
	#define NRF24L01_CE_LOW		MODULE_CE_PORT	&= ~(1<<MODULE_CE_PIN)
	#define NRF24L01_CSN_HIGH	MODULE_CSN_PORT	|= (1<<MODULE_CSN_PIN)
	#define NRF24L01_CSN_LOW	MODULE_CSN_PORT	&= ~(1<<MODULE_CSN_PIN)
	
	//Presets from wireless.h
	#if WIRELESS_TX_PWR == -6
		#define NRF24L01_PRESET_TXPWR 0b10
	#endif
	#if WIRELESS_TX_PWR == -12
		#define NRF24L01_PRESET_TXPWR 0b01
	#endif
	#if WIRELESS_TX_PWR == -18
		#define NRF24L01_PRESET_TXPWR 0b00
	#endif
	#if WIRELESS_TX_PWR == 0
		#define NRF24L01_PRESET_TXPWR 0b11
	#endif
	
	#if WIRELESS_BAUDRATE == 2000
		#define NRF24L01_PRESET_BAUDRATE_HIGH	1
		#define NRF24L01_PRESET_BAUDRATE_LOW	0
	#endif
	#if WIRELESS_BAUDRATE == 1000
		#define NRF24L01_PRESET_BAUDRATE_HIGH	0
		#define NRF24L01_PRESET_BAUDRATE_LOW	0
	#endif
	#if WIRELESS_BAUDRATE == 250
		#pragma message "This option works only with NRF24L01+"
		#define NRF24L01_PRESET_BAUDRATE_HIGH	0
		#define NRF24L01_PRESET_BAUDRATE_LOW	1
	#endif
	
	#define NRF24L01_PRESET_RX WIRELESS_RX_ENABLED
	
	//NRF24L01 low functions
	void	NRF24L01_LOW_init_IO(void);
	void	NRF24L01_LOW_set_register(uint8_t regaddr, uint8_t value);
	uint8_t	NRF24L01_LOW_get_register(uint8_t regaddr);
	void	NRF24L01_LOW_write_register(uint8_t regaddr, uint8_t* data, uint8_t len);
	void	NRF24L01_LOW_read_register(uint8_t regaddr, uint8_t* data, uint8_t len);
	uint8_t	NRF24L01_LOW_read_byte(uint8_t cmd);
	
	//NRF24L01 high functions
	extern void		NRF24L01_init(void);
	extern uint8_t	NRF24L01_get_status(void);
	extern void		NRF24L01_send_data(uint8_t* data, uint8_t len);
	extern void		NRF24L01_get_received_data(uint8_t* data, uint8_t len);
	extern uint8_t	NRF24L01_get_pipe_from_status(uint8_t status);
	extern uint8_t	NRF24L01_get_payload_len(uint8_t pipe);
	extern void		NRF24L01_write_ack_payload(uint8_t pipe, uint8_t* data, uint8_t len);
	extern void		NRF24L01_activate(void);
	extern void		NRF24L01_set_enabled_pipes(nrf24l01_en_rxaddr_t* pipes);
	extern void		NRF24L01_set_tx(void);
	extern void		NRF24L01_set_rx(void);
	extern void		NRF24L01_set_tx_addr(uint8_t* addr, uint8_t len);
	extern void		NRF24L01_set_tx_addr(uint8_t* addr, uint8_t len);
	extern void		NRF24L01_set_rx_addr(uint8_t pipe, uint8_t* addr, uint8_t len);
	extern void		NRF24L01_set_autoack_pipes(nrf24l01_shockburst_t* pipes);
	extern void		NRF24L01_set_payload_width(uint8_t pipe, uint8_t width);
	extern void		NRF24L01_flush_rx();
	extern void		NRF24L01_flush_tx();
	extern void		NRF24L01_set_channel(uint8_t channel);
	
	#include "NRF24L01.c"
#endif