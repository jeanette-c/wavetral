/* ac_convert_wave_wave.cpp - Definition of the wave to wave converter,
 * a.k.a. resampler
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

#include "ac_convert_wave_wave.hpp"
#include <cmath>
#include <samplerate.h>

using std::string;
using std::abs;

namespace JBS {
	Ac_convert_wave_wave::Ac_convert_wave_wave(double zero_threshold, unsigned long int output_size):
		Ac_convert_base(zero_threshold, output_size)
	{}

	Ac_convert_wave_wave::Ac_convert_wave_wave(unsigned long int output_size):
		Ac_convert_base(output_size)
	{}

	// The conversion method: resample the input waveform using libsamplerate
	bool Ac_convert_wave_wave::convert()
	{
		bool convert_state = false;

		// Only begin if the state is OK and input is not nullptr
		if (its_good == true)
		{
			if (its_input != nullptr)
			{
				// Allocate the output data array and data for the resampling
				// process (see libsamplerate simple API)
				its_output = new double[its_size];
				SRC_DATA resampler; // Data struct for the resampling process
				float *local_in, *local_out; // local input/output data
				local_in = new float[its_src_size];
				local_out = new float[its_size];
				double convert_ratio = double(its_size) / double(its_src_size);

				// Copy its_input to local_in
				for (unsigned long int item = 0;item<its_src_size;item++)
				{
					local_in[item] = float(its_input[item]);
				}

				// Configure/fill the resampling data structure
				resampler.input_frames = its_src_size; // samples in input
				resampler.output_frames = its_size; // desired samples/size of output
				resampler.data_in = local_in;
				resampler.data_out = local_out;
				resampler.src_ratio = convert_ratio;
				
				// Convert/resample the input waveform
				int convert_result = src_simple(&resampler, SRC_SINC_BEST_QUALITY, 1);
				if (convert_result != 0) // resampling error
				{
					its_error_msg = string(src_strerror(convert_result));
					its_good = false;
				}
				else // conversion successful
				{
					double max_value = 0.0; // used for rescaling
					// Copy local output to global output
					for (unsigned long int item = 0;item<its_size;item++)
					{
						its_output[item] = double(local_out[item]);
						if (abs(its_output[item]) > max_value)
						{
							max_value = abs(its_output[item]);
						}
					}

					// Rescale output wave to a maximum amplitude of 1
					for (unsigned long int item = 0;item<its_size;item++)
					{
						its_output[item] /= max_value;
					}
					its_ready = true;
					convert_state = true;
				}

				// Free local data arrays
				delete [] local_in;
				delete [] local_out;
			} // if (its_input != nullptr)
		} // if (its_good == true)

		return convert_state;
	}
} // End of namespace JBS
