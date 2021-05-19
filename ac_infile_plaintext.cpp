/* ac_infile_plaintext.cpp - Definition of the plaintext input file class
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

#include <list>
#include "ac_infile_plaintext.hpp"

using std::string;
using std::list;
using std::ifstream;

namespace JBS {
	Ac_infile_plaintext::Ac_infile_plaintext(const string& filename):
		Ac_infile_base(filename)
	{}

	Ac_infile_plaintext::Ac_infile_plaintext(const char * filename):
		Ac_infile_base(filename)
	{}
	Ac_infile_plaintext::~Ac_infile_plaintext()
	{
		if (its_open == true)
		{
			close();
		}
	}

	// Open the connected file
	bool Ac_infile_plaintext::open()
	{
		bool file_state = false;
		if (its_open == true) // If the file is open, just return true
		{
			file_state = true;
		}
		else // Open the file, if its_good is true (i.e. no previous errors)
		{
			if (its_good == true)
			{
				its_file.open(its_filename);
				if (its_file.fail() == true) // open failed, failbit was set
				{
					its_good = false;
					its_error_msg = string("Can't open file ") + its_filename + string(": unknown error.");

					// Make sure that the file is closed upon error
					if (its_file.is_open() == true)
					{
						its_file.close();
					}
				}
				else // file is OK
				{
					its_open = true;
					its_good = true;
					file_state = true;
				}
			} // If there were previous errors, just return false
		}
		return file_state;
	}

	// Close the connected file
	void Ac_infile_plaintext::close()
	{
		if (its_file.is_open() == true)
		{
			its_file.close();
			its_open = false;
		}
	}

	// Read the data from the file and store it in the its_data array
	bool Ac_infile_plaintext::process()
	{
		bool process_state = false;

		// Check that the file is open and ready for reading
		if (its_open == true)
		{
			// Check that the state is good (no prior failures)
			if (its_good == true)
			{
				// Read the contents of the file into a list
				list<double> value_list;
				double cur_value = 0.0;
				string cur_line; // line as characters

				// Read until end-of-file is reached
				while (its_file.eof() != true)
				{
					getline(its_file,cur_line);

					if (!cur_line.empty()) // only convert non-empty lines
					{
						cur_value = stod(cur_line); // convert to float value
						value_list.push_back(cur_value); // add to list
					}
				}

				// Now allocate the data array
				its_data = new double[value_list.size()];

				// Copy the list to its_data array
				unsigned int index = 0;
				for (auto value: value_list)
				{
					its_data[index] = value;
					index++;
				}

				// Set process state to true, it was successful and data size
				its_size = value_list.size();
				process_state = true;
			} // if (its_good == true)
		} // if(its_open == true)

		return process_state;
	}

} // End of namespace JBS
