/*--------------------------------------------------------------------
 Copyright (c) 2016 Alorim Technology.  All right reserved.
 The XLR8Build/XLR8BuildTemplate library provides a path
   to build FPGA images within the Arduino Framework. There
   isn't any C/C++ code associated with this library but
   this empty header file prevents the Arduino IDE from 
   calling it an invalid library
 Written by Matt Weber (support@aloriumtech.com) of
 Alorium Technology (info@aloriumtech.com)

 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library.  If not, see
 <http://www.gnu.org/licenses/>.
 --------------------------------------------------------------------*/

#ifndef _XLR8__SPK_H_INCLUDED
#define _XLR8_SPK_H_INCLUDED

#include <Arduino.h>



#define XLR8_SPK_CTRL _SFR_MEM8(0xE0)

#define XLR8_SPK_DATA _SFR_MEM8(0xE1)



class XLR8_LFSRClass {
	public:
	XLR8_LFSRClass() {}
	~XLR8_LFSRClass() {}
	void __attribute__ ((noinline)) set_seed(uint8_t seed) {
		XLR8_LFSR_SEED = seed;
	}
	uint8_t __attribute__ ((noinline)) get_lfsr() {
		return XLR8_LFSR_DATA;
	}
	void set_freerunning_mode(boolean freerunning) {
		XLR8_LFSR_CTRL = XLR8_LFSR_CTRL | freerunning;
	}
	void set_long_heartbeat(boolean long_heartbeat) {
		XLR8_LFSR_CTRL = XLR8_LFSR_CTRL | (long_heartbeat << 1);
	}
	private:
};

extern XLR8_LFSRClass XLR8_LFSR;
#endif