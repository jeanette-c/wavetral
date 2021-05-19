/* ac_utility.hpp - Declaration of a few utilities (functions and enums)
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

#ifndef AC_UTILITY_HPP
#define AC_UTILITY_HPP

// provide utility functions for all classes

namespace JBS {
	// Convert two chars from an array to a single double, using the Waldorf
	// Microwave II/XT's format
	double ac_convert_mw2_sample(char *data, unsigned int index);

	// Convert a double sample to two bytes in Waldorf Microwave II/XT
	// format and write them to a given index in a char array
	void ac_convert_sample_mw2(double sample, char *sysex, unsigned int index);

	// Convert two chars from an array to one sample, using Microwave I's format
	double ac_convert_mw1_sample(char *data, unsigned int index);

	// Calculate a SysEx checksum Waldorf style
	char ac_get_mw_checksum(char *data, unsigned int start, unsigned int end);

	// Check if the supplied SysEx string starts with the Waldorf Microwave
	// II/XT header
	bool ac_check_header_mw2(char *sysex, unsigned int size);

	// Enum defining what a format can store
	enum class Ac_data_type
	{
		harm, // harmonics only
		wave, // waveform only
		both // both harmonics and waves
	}; // End of enum Ac_data_type
} // End of namespace JBS

#endif // #ifndef AC_UTILITY_HPP
