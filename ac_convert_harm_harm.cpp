/* ac_convert_harm_harm.cpp - Class definition of the converter from spectrum
 * to spectrum, allowing to reduce/increase the harmonics
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

#include "ac_convert_harm_harm.hpp"
#include <cmath>

using std::string;
using std::abs;

namespace JBS {
	Ac_convert_harm_harm::Ac_convert_harm_harm(double zero_threshold, unsigned long int output_size):
		Ac_convert_base(zero_threshold, output_size)
	{}

	Ac_convert_harm_harm::Ac_convert_harm_harm(unsigned int long output_size):
		Ac_convert_base(output_size)
	{}

	// The convert function
	bool Ac_convert_harm_harm::convert()
	{
		bool convert_state = false;
		// Only perform if the state is OK
		if (its_good == true)
		{
			if (its_input != nullptr)
			{
				// Allocate output data
				its_output = new double[its_size];

				// Copy data from the input, if output is larger, pad with zeroes
				unsigned long int max_item;
				// Set the endpoint for the for-loop
				if (its_src_size < its_size)
				{
					max_item = its_src_size;
					// Add the zero padding for higher harmonics
					for (unsigned long int item = its_src_size;item<its_size;item++)
					{
						its_output[item] = 0.0;
					}
				}
				else
				{
					max_item = its_size;
				}

				// Copy data
				for (unsigned long int item = 0;item<max_item;item++)
				{
					if (abs(its_input[item]) <= its_zero_threshold)
					{
						its_output[item] = 0.0;
					}
					else
					{
						its_output[item] = its_input[item];
					}
				}

				// The job is complete, set convert_state and its_ready to true
				its_ready = true;
				convert_state = true;
			} // if (its_input != nullptr)
			else
			{
				its_good = false;
				its_error_msg = string("There is no input, although input for conversion.");
			}
		} // if (its_good == true)

		return convert_state;
	}
} // End of namespace JBS
