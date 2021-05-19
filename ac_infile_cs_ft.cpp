/* ac_infile_cs_ft.cpp - Definition of the Csound ftsave-format ftable input
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

#include "ac_infile_cs_ft.hpp"

using std::string;
using std::ifstream;

namespace JBS {
	Ac_infile_cs_ft::Ac_infile_cs_ft(const string& filename):
		Ac_infile_base(filename)
	{}

	Ac_infile_cs_ft::Ac_infile_cs_ft(const char *filename):
		Ac_infile_base(filename)
	{}

	Ac_infile_cs_ft::~Ac_infile_cs_ft()
	{
		if (its_open == true)
		{
			close();
		}
	}

	// Open the connected file
	bool Ac_infile_cs_ft::open()
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
	void Ac_infile_cs_ft::close()
	{
		if (its_file.is_open() == true)
		{
			its_file.close();
			its_open = false;
		}
	}

	// Read the data from the file and store it in the its_data array
	bool Ac_infile_cs_ft::process()
	{
		bool process_state = false;
		// Check that the file is open and ready for reading
		if (its_open == true)
		{
			// Check that the state is good (no prior failures)
			if (its_good == true)
			{
				// Check for the required header information
				string cur_line;
				unsigned long int num_values = 0;
				getline(its_file,cur_line);
				if (cur_line.compare(0,14,"======= TABLE ") != 0)
				{ // It's not a Csound table saved with ftsave or ftsavek
					its_good = false;
					its_error_msg = its_filename + string(" is not a Csound ftsave/ftsavek file.");
					return process_state;
				}

				// Check for length(number of values) and channels(only 1)
				// repeat as until end of header or end of file is reached
				while ((its_file.eof() != true) && (cur_line.compare("---------END OF HEADER--------------") != 0))
				{
					getline(its_file,cur_line);
					if (cur_line.compare(0,9,"flenfrms:") == 0)
					{ // read the number of values
						num_values = stoul(cur_line.substr(9));
					}
					else if (cur_line.compare(0,7,"nchnls:") == 0)
					{ // Read the number of channels, only 1 is acceptable
						if (stoi(cur_line.substr(7)) != 1)
						{
							its_good = false;
							its_error_msg = its_filename + string("contains more than one channel.");
						}
					}
				}

				// Check its_good to see if the table is in acceptable format
				// and EOF, so the file is not too short
				if ((its_good == true) && (its_file.eof() == false))
				{
					// allocate the data array and read values into it
					its_data = new double[num_values];
					for (int item = 0;item<num_values;item++)
					{
						if (its_file.eof() == true) // premature end of file
						{
							its_good = false;
							delete [] its_data;
							its_data = nullptr;
							its_error_msg = its_filename + string(" is corrupt, there are not enough values.");
							break;
						}
						getline(its_file,cur_line);
						if (!cur_line.empty()) // only process non-empty lines
						{
							its_data[item] = stod(cur_line);
						}
					}

					if (its_good == true) // could not be OK on premature EOF
					{
						its_size = num_values;
						process_state = true; // processing was successful
					}
				} // file was good for reading values
			} // File existed
		} // File was open

		return process_state;
	}
} // end of namespace JBS
