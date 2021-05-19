/* ac_utility.cpp - Definition of a few utilities, including helper function
 * and enums
 * Copyright (C) 2021 Jeanette C. <jeanette@juliencoder.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "ac_utility.hpp"
#include <cmath>

using std::round;

namespace JBS {
	// Convert two chars from an array to a single double, using the Waldorf
	// Microwave II/XT's format
	double ac_convert_mw2_sample(char *data, unsigned int index)
	{
		int int_value = ((data[index] * 16) + data[index + 1])^0x80;
		if (int_value & 0x80) // signed extension
		{
			int_value -= 256;
		}
		return double(int_value);
	}

	// Convert two chars from an array to one sample, using Microwave I's format
	double ac_convert_mw1_sample(char *data, unsigned int index)
	{
		int int_value = ((data[index] * 16) + data[index + 1]);
		return double(int_value);
	}

	// Convert a double sample two bytes in Waldorf Microwave II/XT format
	// Assume that samples have been scaled to a maximum of 1 by any converter
	void ac_convert_sample_mw2(double sample, char *sysex, unsigned int index)
	{
		int int_sample = int(round(sample * 127));
		int_sample ^=0x80;
		sysex[index] = ((int_sample >> 4) & 0x0f);
		sysex[index + 1] = (int_sample & 0x0f);
	}

	// Calculate a SysEx checksum Waldorf style: add all chars, allowing overflow
	// and limit the output to 0-0x7f by doing &0x7f
	char ac_get_mw_checksum(char *data, unsigned int start, unsigned int end)
	{
		char checksum = 0x00;
		for (unsigned int index = start;index<end;index++)
		{
			checksum = char(checksum + data[index]);
		}
		checksum &=0x7f;
		return checksum;
	}

	// Check if the supplied SysEx strings begins with the Waldorf Microwave
	// II/XT header
	bool ac_check_header_mw2(char *sysex, unsigned int size)
	{
		bool is_mw2 = true;
		const char mw2_header[] = { char(0xf0), char(0x3e), char(0x0e) };
		if (size > 3)
		{
			for (unsigned int index = 0;index < 3; index ++)
			{
				if (sysex[index] != mw2_header[index])
				{
					is_mw2 = false;
					break;
				}
			}
		}
		else // size can't fit header: it's no mw2 file
		{
			is_mw2 = false;
		}

		return is_mw2;
	}
} // End of namespace JBS
