/* ac_outfile_yoshimi.cpp - Definition of the Yoshimi script output file class
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

#include "ac_outfile_yoshimi.hpp"
#include <cmath>

using std::string;
using std::ofstream;
using std::round;

namespace JBS {
	Ac_outfile_yoshimi::Ac_outfile_yoshimi(const string& filename):
		Ac_outfile_base(filename)
	{}

	Ac_outfile_yoshimi::Ac_outfile_yoshimi(const char *filename):
		Ac_outfile_base(filename)
	{}

	Ac_outfile_yoshimi::~Ac_outfile_yoshimi()
	{
		if (its_open == true)
		{
			close();
		}
	}

	// Open the connected file for writing
	bool Ac_outfile_yoshimi::open()
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

	void Ac_outfile_yoshimi::close()
	{
		if (its_open == true)
		{
			its_file.close();
			its_open = false;
		}
	}

	bool Ac_outfile_yoshimi::process()
	{
		bool process_state = false;

		// Begoin writing if state is good, file is open and data is there
		if (its_open == true)
		{
			if (its_data != nullptr)
			{
				if (its_good == true)
				{
					// Convert up to 128 double amplitudes to short ints between
					// -63..+63
					unsigned long int max_item = 0;

					// Find the maximum number of harmonics: 128 or less
					if (its_size < 128)
					{
						max_item = its_size;
					}
					else // Yoshimi can't take more than 128 harmonics
					{
						max_item = 128;
					}

					// Scale and round the amplitudes, then write to script
					// Script lines take the form:
					// s ha index a amplitude
					// Remember: all converters rescale to a maximum of 1
					int current_harmonic = 0; // harmonic amplitude to write
					for (unsigned short int item = 0;item<max_item;item++)
					{
						current_harmonic = int(round(its_data[item] * 63));
						if (current_harmonic != 0)
						{ // Write a line
							its_file << "s ha " << (item + 1) << " a " << (current_harmonic + 64) << "\n";
						}
					}

					// Done processing
					process_state = true;
				} // if (its_good == true) // begin process if no other errors
			} // if (its_data != nullptr) - being process when data is present
		} // if (its_open == true) - begin process if file is open

		return process_state;
	}
} // End of namespace JBS
