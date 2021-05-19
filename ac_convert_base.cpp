/* ac_convert_base.cpp - Definition of the base class for all converters
 * between harmonic spectra and waveforms
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

#include "ac_convert_base.hpp"

using std::string;

namespace JBS {
	Ac_convert_base::Ac_convert_base(double zero_threshold, unsigned long int output_size):
		its_zero_threshold(zero_threshold), its_size(output_size), \
		its_ready(false), its_good(true), its_error_msg(""), \
		its_input(nullptr), its_output(nullptr), its_src_size(0)
	{}

	Ac_convert_base::Ac_convert_base(unsigned long int output_size):
		Ac_convert_base(0, output_size)
	{}

	Ac_convert_base::~Ac_convert_base()
	{
		if (its_input != nullptr)
		{
			delete [] its_input;
		}

		if (its_output != nullptr)
		{
			delete [] its_output;
		}
	}

	// Retrieve the output data
	double* Ac_convert_base::get_output()
	{
		// Only return something, if the output is ready and apparently correct
		if ((its_ready == true) && (its_good == true))
		{
			return its_output;
		}
		else // either not ready or incorrect, return null pointer
		{
			return nullptr;
		}
	}

	void Ac_convert_base::set_input(double *input, unsigned long int size)
	{
		its_input = new double[size];
		for (unsigned long int item = 0;item<size;item++)
		{
			its_input[item] = input[item];
		}
		its_src_size = size;
	}
} // End of namespace JBS
