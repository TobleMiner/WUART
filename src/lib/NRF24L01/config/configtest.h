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

//Check for F_CPU
#ifndef F_CPU
	#error "To use this wireless lib F_CPU must be defined"
#endif

//Wireless stuff
#if WIRELESS_CHANNEL > 125 || WIRELESS_CHANNEL < 0
	#error "Invalid wireless channel. Your wireless channel must be inside [0 ... 125]"
#endif

#if WIRELESS_BAUDRATE != 250 && WIRELESS_BAUDRATE != 1000 && WIRELESS_BAUDRATE != 2000
	#error "Unsupported wireless baudrate. Must be either 250 [kbps], 1000 [kbps] or 2000 [kbps]"
#endif

#if WIRELESS_PACK_LEN < 0 || WIRELESS_PACK_LEN > 32
	#error "Invalid wireless paket length. Must be inside [0 ... 32]"
#endif

#if WIRELESS_TX_PWR < -18 || WIRELESS_TX_PWR > 0 || WIRELESS_TX_PWR % 6 != 0
	#error "Invalid wireless TX power. Must be either -18 [dBm], -12 [dBm], -6 [dBm] or 0 [dBm]"
#endif

#if WIRELESS_TX_ENABLED == TRUE && WIRELESS_RX_ENABLED == TRUE
	#pragma message "Keep in mind that you will have to switch between transmit and receive mode manually."
#endif