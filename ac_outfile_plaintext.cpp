/* ac_outfile_plaintext.cpp - Definition of the plaintext output file class
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

#include "ac_outfile_plaintext.hpp"

using std::string;
using std::ofstream;
using std::endl;

namespace JBS {
	Ac_outfile_plaintext::Ac_outfile_plaintext(const string& filename):
		Ac_outfile_base(filename)
	{}

	Ac_outfile_plaintext::Ac_outfile_plaintext(const char *filename):
		Ac_outfile_base(filename)
	{}

	Ac_outfile_plaintext::~Ac_outfile_plaintext()
	{
		if (its_open == true)
		{
			close();
		}
	}

	// Open the file
	bool Ac_outfile_plaintext::open()
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

	// Close connected file
	void Ac_outfile_plaintext::close()
	{
		if (its_open == true)
		{
			its_file.close();
			its_open = false;
		}
	}

	// Write the data to the file
	bool Ac_outfile_plaintext::process()
	{
		bool process_state = false;

		// Begoin writing if state is good, file is open and data is there
		if (its_open == true)
		{
			if (its_data != nullptr)
			{
				if (its_good == true)
				{
					// Write data as plain text
					for (unsigned long int item = 0;item<(its_size-1);item++)
					{
						its_file << its_data[item] << endl;
					}
					// Write the last value without newline
					its_file << its_data[(its_size -1)];
					its_file.flush();
					process_state = true; // Success
				} // if (its_good == true)
			}
			else // No data
			{
				its_error_msg = string("There is no data to write to file ") + its_filename;
			}
		}
		else // Fiole not open
		{
			its_error_msg = string("Can't write data, file ") + its_filename + string(" is not open.");
		}

		return process_state;
	}
} // End of namespace JBS
