/* ac_infile_audio.cpp - Definition of the audiofile input class
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

#include "ac_infile_audio.hpp"

using std::string;
using std::to_string;

namespace JBS {
	Ac_infile_audio::Ac_infile_audio(const string& filename):
		Ac_infile_base(filename)
	{
		its_file = NULL;
	}

	Ac_infile_audio::Ac_infile_audio(const char *filename):
		Ac_infile_base(filename)
	{
		its_file = NULL;
	}

	Ac_infile_audio::~Ac_infile_audio()
	{
		if (its_open == true)
		{
			close();
		}
	}

	// Open the connected file
	bool Ac_infile_audio::open()
	{
		bool file_state = false;
		if (its_open == true)
		{ // Do nothing
			file_state = true;
		}
		else // Open the file
		{
			// Open the file and get information
			its_file = sf_open(its_filename.c_str(), SFM_READ, &its_info);
			if (its_file == NULL)
			{ // open error
				its_error_msg = string(sf_strerror(its_file));
				its_good = false;
			}
			else // File was opened successfully
			{
				file_state = true;
				its_open = true;
				its_good = true;
				// Only allow mono input files
				if (its_info.channels != 1)
				{
					its_error_msg = its_filename + string(" has more than one audio channel.");
					its_good = false;
					sf_close(its_file);
					its_open = false;
					its_file = NULL;
				}

				// Only allow up to 65536 samples
				if (its_info.frames > 65536)
				{
					its_error_msg = its_filename + string(" has too many samples.");
					its_good = false;
					sf_close(its_file);
					its_file = NULL;
					its_open = false;
				}
			} // else file was opened successfully
		} // if(its_open == true)

		return file_state;
	}

	void Ac_infile_audio::close()
	{
		if (its_open == true)
		{
			sf_close(its_file);
			its_file = NULL;
			its_open = false;
		}
	}

	// Read the data and process it
	bool Ac_infile_audio::process()
	{
		bool process_state = false;

		// Check that the file is open and ready for reading
		if (its_open == true)
		{
			// Check that the file status is good
			if (its_good == true)
			{
				// The whole file is one waveform
				// So check the length to allocate output data
				sf_count_t num_values = its_info.frames;
				its_data = new double[num_values];
				sf_count_t read_frames = 0;
				read_frames = sf_readf_double(its_file,its_data,num_values);
				if (read_frames != num_values)
				{
					its_error_msg = string("Only read ") + to_string(read_frames) + string(" of ") + to_string(num_values) + string(" samples.");
				}
				else // all was well, set process_state to true
				{
					its_size = num_values;
					process_state = true;
				}
			} // if (iots_good == true)
		} // if (its_open == rue)

		return process_state;
	}
} // End of namespace JBS
