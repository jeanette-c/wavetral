/* ac_outfile_mw2.cpp - Definition of the Microwave II/XT wave dump output
 * file class
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

#include "ac_outfile_mw2.hpp"
#include "ac_utility.hpp"
#include <cmath>

using std::string;
using std::to_string;
using std::sin;
using std::abs;

namespace JBS {
	Ac_outfile_mw2::Ac_outfile_mw2(const string& filename):
		Ac_outfile_base(filename), its_sysex_string(nullptr), its_ready(false)
	{
		its_item = 1000;
	}

	Ac_outfile_mw2::Ac_outfile_mw2(const char *filename):
		Ac_outfile_base(filename), its_sysex_string(nullptr), its_ready(false)
	{
		its_item = 1000;
	}

	Ac_outfile_mw2::~Ac_outfile_mw2()
	{
		if (its_open == true)
		{
			close();
		}

		if (its_sysex_string != nullptr)
		{
			delete [] its_sysex_string;
		}
	}

	// Open the connected file
	bool Ac_outfile_mw2::open()
	{
		bool file_state = false;

		if (its_open == true) // file is open, nothing to do
		{
			file_state = true;
		}
		else // File is not open
		{
			if (its_good == true) // file can be opened for writing
			{
				its_file.open(its_filename);
				if (its_file.fail() == true)
				{
					its_good = false;
					its_error_msg = string("Can't open file ") + its_filename + string(": unknown error.");
					// Make sure that the file is closed
					if (its_file.is_open() == true)
					{
						its_file.close();
					}
				}
				else // File is OK
				{
					its_open = true;
					its_good = true;
					file_state = true;
				}
			} // if (its_good == true) all OK to begin open
		} // if (its_open == true)

		return file_state;
	}

	void Ac_outfile_mw2::close()
	{
		if (its_open == true)
		{
			its_file.close();
			its_open = false;
		}
	}

	// Internal method to construct the wave from the input harmonics
	void Ac_outfile_mw2::make_wave()
	{
		// Allocate the necessary data arrays
		its_sysex_string = new char[137]; // string to be written to file
		double *samples = new double[64]; // temporary waveform
		// Initialise samples with 0 values
		for (unsigned int index = 0;index<64;index++)
		{
			samples[index] = 0.0;
		}

		// Find the maximum harmonic to write: 64 if present or less
		unsigned int max_harmonic = 0;
		double max_value = 0.0; // used for rescaling
		if (its_size >= 64)
		{
			max_harmonic = 64;
		}
		else
		{
			max_harmonic = its_size;
		}

		// Create a half cycle
		for (unsigned int sample = 0;sample<64;sample++)
		{
			for (unsigned int harmonic = 0;harmonic<max_harmonic;harmonic++)
			{
				samples[sample] += sin((2 * M_PI * (sample + 0.5) * (harmonic + 1)) / 128) * its_data[harmonic];
			}

			// Keep track of maximum amplitude
			if (abs(samples[sample]) > max_value)
			{
				max_value = abs(samples[sample]);
			}
		} // end for samples loop

		// Rescale the wave to an amplitude of 1
		for (unsigned int sample = 0;sample<64;sample++)
		{
			samples[sample] /= max_value;
		}

		// Convert the double samples to entries in the SysEx string
		for (unsigned int index = 0;index<64;index++)
		{
			ac_convert_sample_mw2(samples[index], its_sysex_string, (7 + (index * 2)));
		}

		// Clean up
		delete [] samples;
		its_ready = true; // there is wave-data to write
	}

	bool Ac_outfile_mw2::process()
	{
		bool process_state = false;

		// Begin processing if state is good and file is open
		if (its_good == true)
		{
			if (its_open == true)
			{
				// Wave number (its_item) must be greater 1000 for a user wave
				if (its_item >=1000)
				{
					// Make the wave form
					make_wave();
					if (its_ready == true) // if wave creation was successful
					{
						// Write header
						its_sysex_string[0] = char(0xf0); // SysEx start
						its_sysex_string[1] = char(0x3e); // Waldorf
						its_sysex_string[2] = char(0x0e); // Microwave II/XT
						its_sysex_string[3] = char(0x7f); // broadcast Device ID
						its_sysex_string[4] = char(0x12); // Wave dump (WAVD)
						its_sysex_string[5] = char((its_item >> 7) & 0x7f); // number high nibble
						its_sysex_string[6] = char(its_item & 0x7f); // number low nibble
						// Calculate checksum
						char chksum = ac_get_mw_checksum(its_sysex_string, 7, 134);
						its_sysex_string[135] = chksum;
						its_sysex_string[136] = char(0xf7); // End of sysEx

						// Write SysEx string to file
						its_file.write(its_sysex_string,137);

						// Finished successfully
						process_state = true;
					} // if (its_ready == true) - waveform was created
				} // if (its_item >=1000) - if this is a user wave number
				else // Wave can't be dumped to Microwave
				{
					its_good = false;
					its_ready = false;					its_error_msg = string("Can't write wave number ") + to_string(its_item) + string(" It's not a user wave. User waves start at 1000.");
				}
			} // if (its_open == true) - begin if file is open
		} // if (its_good == true) - begin when state is good

		return process_state;
	}
} // End of namespace JBS
