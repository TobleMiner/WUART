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

#ifndef __WIRELESS_CFG__
#define __WIRELESS_CFG__
	//Main config
	#define WIRELESS_CHANNEL	42		//Initial com channel, range 0 ... 125
	#define WIRELESS_BAUDRATE	1000	//in kbps. Enter either 250, 1000 or 2000. everything else will lead to a compiler error as it is not supported by the NRF24L01+ series wireless modules
	#define WIRELESS_PACK_LEN	32		//Packet length in bytes used for data transmissions. Value must be inside [0 ... 32]. If set to 0 dynamic payload length is enabled
	#define WIRELESS_EN_TX_IRQ	TRUE
	#define WIRELESS_EN_RX_IRQ	TRUE
	#define WIRELESS_EN_RT_IRQ	TRUE
	
	//TX config
	#define WIRELESS_TX_ENABLED TRUE
	#define WIRELESS_TX_PWR		0		//TX power in dBm. Must be either 0, -6, -12 or -18
	
	//RX config
	#define WIRELESS_RX_ENABLED TRUE
#endif