/* ac_infile_mw2.cpp - Definition of the Waldorf Microwave II/XT wave dump
 * input class
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

#include "ac_infile_mw2.hpp"
#include "ac_utility.hpp"
#include <cstring>

using std::string;
using std::ifstream;
using std::list;
using std::vector;
using std::strstr;
using std::to_string;

namespace JBS {
	Ac_infile_mw2::Ac_infile_mw2(const string& filename):
		Ac_infile_base(filename), its_sysex_string(nullptr), its_item_count(0), \
		its_ready(false), its_file_size(0)
	{}

	Ac_infile_mw2::Ac_infile_mw2(const char *filename):
		Ac_infile_base(filename), its_sysex_string(nullptr), its_item_count(0), \
		its_ready(false), its_file_size(0)
	{}

	Ac_infile_mw2::~Ac_infile_mw2()
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

	// Open the file and initialise the item count
	bool Ac_infile_mw2::open()
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
					read_data(); // read the SysEx string
					count_items(); // if item count is 0 it's not a valid file
					if (its_item_count == 0)
					{
						its_file.close();
						its_open = false;
						its_good = false;
						its_error_msg = its_filename + string(" does not contain valid Microwave II/XT waveforms.");
					}
					else // The file is acceptable
					{
						its_open = true;
						its_good = true;
						file_state = true;
					}
				}
			} // if (its_good == true) all OK to begin open
		} // if (its_open == true)

		return file_state;
	}

	// Close the connected file
	void Ac_infile_mw2::close()
	{
		if (its_open == true)
		{
			its_file.close();
			its_open = false;
			its_ready = false;
		}
	}

	// Read the SysEx string into internal data and verify the file format
	void Ac_infile_mw2::read_data()
	{
		// Only begin if the file is open
		if (its_open == true)
		{
			// Get the file size
			its_file.seekg(0,std::ios_base::end); // go to end of file
			std::streampos length = its_file.tellg(); // get the end position
			its_file.seekg(0,std::ios_base::beg); // Go back to beginning

			// Allocate storage for the complete data
			its_sysex_string = new char[length];

			// Read the whole file into the SysEx string
			its_file.read(its_sysex_string, length);

			// Check that the string contains the Microwave II/XT header
			if (ac_check_header_mw2(its_sysex_string, length) != true)
			{
				its_good = false;
				its_error_msg = its_filename + string(" is not a Microwave II/XT SysEx file.");
				delete [] its_sysex_string;
				its_sysex_string = nullptr;
			}
			else // it's a microwave II/XT file
			{
				its_file_size = length;
			}
		} // if (its_open == true)
	}

	// Count items in the file
	void Ac_infile_mw2::count_items()
	{
		// Begin if object state is good and sysEx data is present
		if (its_good == true)
		{
			if (its_sysex_string != nullptr)
			{
				const char mw_header[] = { char(0xf0), char(0x3e), char(0x0e) };
				char *tmp_pos, *found_pos;
				tmp_pos = its_sysex_string; // incremented in search
				unsigned int found_index = 0;
				unsigned int found_wave_number = 0;

				// Search for waves to the end of the string
				while ((found_pos = strstr(tmp_pos, mw_header)) != nullptr)
				{
					found_index = int(found_pos - its_sysex_string);
					if ((found_index + 6) < its_file_size) // enough for wavd and location
					{
						// check for 0x12 WAVD (waveform dump) command
						if (its_sysex_string[found_index + 4] == 0x12) // it's a wave
						{
							found_wave_number = int(its_sysex_string[found_index + 5] * 128 + its_sysex_string[found_index + 6]);
							found_index += 7;
							its_item_numbers.push_back(found_wave_number);
							its_item_indices.push_back(found_index);
							its_item_count++;
						} // if found pos + 2 is a WAVD command
					} // if ((found_index + 2) < its_file_size)
					tmp_pos = ++found_pos;
				} // end of while mw_header is found loop
			} // if (its_sysex_string != nullptr)
		} // if (its_good == true)

		if (its_item_count >0) // waves were found, it's good
		{
			its_ready = true;
		}
		else // this file does not contain waves
		{
			its_good = false;
			its_error_msg = its_filename + string(" does not contain any waveforms.");
		}
	}

	// Return a list of wave names (either ROM#index or USER#index
	list<string> Ac_infile_mw2::get_item_list()
	{
		list<string> wave_list;
		string current_name;

		// Only begin processing if its_ready is true and its_item_count >0
		if (its_ready == true)
		{
			if (its_item_count >0) // if there are any waves
			{
				unsigned int list_size = its_item_count;
				for (unsigned int item = 0;item<list_size;item++)
				{
					if (its_item_numbers[item] <300) // ROM wave
					{
						current_name = to_string(item) + string(" ROM#") + to_string(its_item_numbers[item] + 1);
					}
					else if (its_item_numbers[item] >= 1000)
					{
						current_name = to_string(item) + string(" USER#") + to_string(its_item_numbers[item] - 999);
					}
					else // must be an illegal wave
					{
						current_name = to_string(item) + string(" illegal#") + to_string(its_item_numbers[item]);
					}
					wave_list.push_back(current_name);
				} // End of for-loop going through found waves
			} // if (its_count >0)
		} // if (its_ready == true)

		return wave_list;
	}

	// Set number of item to read
	bool Ac_infile_mw2::set_item(unsigned int number)
	{
		bool set_state = false;
		if (number <= its_item_count)
		{
			its_item = number;
			set_state = true;
		}

		return set_state;
	}

	// Read the requested wave into the data array
	bool Ac_infile_mw2::process()
	{
		bool process_state = false;

		// Begin if its_ready true and item number within range
		if (its_ready == true) // there is data
		{
			if (its_item < its_item_count)
			{
				unsigned int wave_start = its_item_indices[its_item];
				unsigned int wave_end = wave_start + 128;

				// Allocate data array
				its_data = new double[128];
				its_size = 128;
				
				// Read the data and convert to double, this will only read
				// a half cycle (see Microwave II/XT wavd format)
				for (unsigned int index = wave_start;index<wave_end;index += 2)
				{
					its_data[index / 2] = ac_convert_mw2_sample(its_sysex_string, index);
				}

				// Generate the other half of the wave by wave[64+n] = -wave[63-n]
				for (unsigned int index = 0;index<64;index++)
				{
					its_data[64 + index] = (-its_data[63 - index]);
				}

				// Process completed successfully
				its_good = true;
				process_state = true;
			} // if (its_item < its_item_count) - begin if item within range
		} // if (its_ready == true) - begin if there is data

		return process_state;
	}
} // End of namespace JBS
