/* ac_convert_wave_harm.cpp - Definition of the wave to harmonic converter
 * (FFT)
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

#include "ac_convert_wave_harm.hpp"
#include <cmath>
#include <fftw3.h>

using std::abs;
using std::hypot;

namespace JBS {
	Ac_convert_wave_harm::Ac_convert_wave_harm(double zero_threshold, unsigned long int output_size):
		Ac_convert_base(zero_threshold, output_size)
	{}

	Ac_convert_wave_harm::Ac_convert_wave_harm(unsigned long int output_size):
		Ac_convert_base(output_size)
	{}

	// The actual conversion function, performing an FFT analysis of
	// the input wave.
	bool Ac_convert_wave_harm::convert()
	{
		bool convert_state = false;

		// Only start when the state is OK and there is input
		if (its_good == true)
		{
			if (its_input != nullptr)
			{
				// Allocate the global output array
				its_output = new double[its_size];
				// Create an allocate two internal data arrays, because FFTW has its
				// own allocation/free methods, geared towards particular performance
				// Both arrays have the size of the input
				double *local_in, *local_out;
				local_in = (double *) fftw_malloc(sizeof(double) * its_src_size);
				local_out = (double *) fftw_malloc(sizeof(double) * its_src_size);
		
				// Create the FFTW plan for analysis: half-complex with real values
				// r[k] in local_out[k] and imaginary values i[k] in local_out[size - k]
				fftw_plan conv_plan = fftw_plan_r2r_1d(its_src_size, local_in, local_out, \
					FFTW_R2HC, FFTW_ESTIMATE);
				// Copy input data to local_in
				for (unsigned long int value = 0;value<its_src_size;value++)
				{
					local_in[value] = its_input[value];
				}

				// Execute the fftw_plan, i.e. perform the FFT analysis
				fftw_execute(conv_plan);
				
				// Now copy amplitudes into the global output array, testing for
				// zero threshold

				double cur_value = 0.0; // temporary current value
				unsigned long int num_bins = (its_src_size / 2);
				unsigned long int max_item = 0; // maximum index for the for-loop
				double max_value = 0.0; // temporary maximum value, used for rescaling
				if (num_bins < its_size)
				{
					max_item = num_bins;

					// Write zero padding for higher harmonics
					for (unsigned long int item = num_bins;item<its_size;item++)
					{
						its_output[item] = 0.0;
					}
				}
				else // No zero padding necessary
				{
					max_item = its_size;
				}
				
				// Write the first max_item amplitudes of the harmonics to the output
				for (unsigned long int item = 0;item<max_item;item++)
				{
					its_output[item] = hypot(local_out[item + 1], local_out[its_src_size - 1 - item]);
					if (abs(its_output[item]) > max_value)
					{
						max_value = abs(its_output[item]);
					}
				}

				// Rescale output to a maximum of 1
				for (unsigned long int item = 0;item<its_size;item++)
				{
					cur_value = its_output[item] / max_value;
					if (abs(cur_value) < its_zero_threshold)
					{
						its_output[item] = 0.0;
					}
					else
					{
						its_output[item] = cur_value;
					}
				}
				
				// Free local data
				fftw_destroy_plan(conv_plan);
				fftw_free(local_in);
				fftw_free(local_out);

				// Conversion successful, set convert_state and its_ready
				convert_state = true;
				its_ready = true;
			} // if (its_input != nullptr)
		} // if (its_good == true)

		return convert_state;
	}
} // End of namespace JBS
