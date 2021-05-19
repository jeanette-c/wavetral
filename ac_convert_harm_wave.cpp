/* ac_convert_harm_wave.cpp - Definition of the harmonic to wave converter
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

#include "ac_convert_harm_wave.hpp"
#include <cmath>

using std::sin;
using std::abs;

namespace JBS {
	Ac_convert_harm_wave::Ac_convert_harm_wave(double zero_threshold, unsigned long int output_size):
		Ac_convert_base(zero_threshold, output_size)
	{}

	Ac_convert_harm_wave::Ac_convert_harm_wave(unsigned long int output_size):
		Ac_convert_base(output_size)
	{}

	bool Ac_convert_harm_wave::convert()
	{
		bool convert_state = false;

		// Only perform if the state is OK
		if (its_good == true)
		{
			if (its_input != nullptr)
			{
				// Allocate output data
				its_output = new double[its_size];
				double max_value = 0.0; // Maximum amplitude, used for rescaling

				// Set output data values to 0
				for (unsigned long sample = 0;sample<its_size;sample++)
				{
					its_output[sample] = 0.0;
				}

				// Create the waveform
				for (unsigned long int sample = 0;sample<its_size;sample++)
				{ // for every sample
					for (unsigned long int harmonic = 0;harmonic<its_src_size;harmonic++)
					{ // every harmonic
						if (abs(its_input[harmonic - 1]) > its_zero_threshold)
						{ // only if greater than zero threshold
							// Write the point of a sine
							its_output[sample] += sin((2 * M_PI * sample * (harmonic + 1)) / its_size) * its_input[harmonic];
						}
					} // for harmonics
					if (abs(its_output[sample]) > max_value) // adjust the maximum
					{
						max_value = abs(its_output[sample]);
					}
				} // for samples

				// Now rescale the wave to max amplitude 1
				for (unsigned long int sample = 0;sample<its_size;sample++)
				{
					its_output[sample] /= max_value;
				}

				// Taks complete, set its_ready and convert_state to true
				its_ready = true;
				convert_state = true;
			} // if (its_input != nullptr)
		} // if (its_good == true)

		return convert_state;
	}
} // End of namespace JBS
