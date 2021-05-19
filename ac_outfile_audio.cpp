/* ac_outfile_audio.cpp - Definition of the audiofile output class
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

#include "ac_outfile_audio.hpp"

using std::string;

namespace JBS {
	Ac_outfile_audio::Ac_outfile_audio(const string& filename):
		Ac_outfile_base(filename), its_format(0), its_file(nullptr)
	{}

	Ac_outfile_audio::Ac_outfile_audio(const char *filename):
		Ac_outfile_base(filename), its_format(0), its_file(nullptr)
	{}

	Ac_outfile_audio::~Ac_outfile_audio()
	{
		if (its_open == true)
		{
			close();
		}
	}

	// Open the connected audio file
	bool Ac_outfile_audio::open()
	{
		bool file_state = false;

		if (its_open == true) // file is open, nothing to do
		{
			file_state = true;
		}
		else // Open the file
		{
			if (its_good == true) // no other internal errors
			{
				// Deduce the filetype from the file extension
				size_t ext_start = its_filename.rfind('.');
				string extension = its_filename.substr(ext_start + 1);
				if (extension.compare("wav") == 0)
				{
					its_format = SF_FORMAT_WAV;
				}
				else if (extension.compare("aiff") == 0)
				{
					its_format = SF_FORMAT_AIFF;
				}
				else
				{
					its_good = false;
					its_error_msg = string("Audio output can only be written in wav and aiff format. Filenames should end in .wav or .aiff.");
				}

				if (its_good == true) // a wav of aiff filename was given
				{
					its_format |= SF_FORMAT_PCM_16; // signed 16bit

					// Now fill in the sf_info structure
					its_info.samplerate = its_samplerate;
					its_info.channels = 1;
					its_info.format = its_format;
					// Now open the file
					its_file = sf_open(its_filename.c_str(), SFM_WRITE, &its_info);
					if (its_file == NULL)
					{
						its_error_msg = string(sf_strerror(its_file));
						its_good = false;
					}
					else // file was opened successfully
					{
						its_open = true;
						file_state = true;
					}
				}
			} // if (its_good == true)
		} // if (its_open == true)

		return file_state;
	}

	// Close the connected audio file
	void Ac_outfile_audio::close()
	{
		if (its_open == true)
		{
			sf_close(its_file);
			its_file = NULL;
			its_open = false;
		}
	}

	// Write the data to the file
	bool Ac_outfile_audio::process()
	{
		bool process_state = false;

		// begin work if state is OK, file is open and data is ready
		if (its_good == true)
		{
			if (its_data != nullptr)
			{
				if (its_open == true)
				{
					// Write the whole data block to audio file
					sf_count_t written_items = 0;
					sf_count_t write_size = its_size;
					written_items = sf_write_double(its_file, its_data, write_size);
					if (written_items != write_size)
					{
						its_good = false;
						its_error_msg = string("Not all data was written to ") + its_filename;
					}
					else // writing successful
					{
						process_state = true;
					}
				}
				else // Output file is not open
				{
					its_error_msg = its_filename + string("Has not been opened for output yet.");
				} // if (its_open == true) - start when file is open
			}
			else // no data yet
			{
				its_error_msg = string("There is no data to write to file.");
			} // if (its_data != nullptr) - start work when data is there
		} // if (its_good == true) - start processing on good state

		return process_state;
	}
} // End of namespace JBS
