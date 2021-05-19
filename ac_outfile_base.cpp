/* ac_outfile_base.cpp - Declaration of the output file base class, not to be
 * instantiated, only as a pointer
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

#include "ac_outfile_base.hpp"
#include <boost/filesystem.hpp>

using std::string;
using boost::filesystem::path;
using boost::filesystem::exists;

namespace JBS {
	Ac_outfile_base::Ac_outfile_base(const string& filename):
		its_filename(filename), its_error_msg(""), its_good(true), \
		its_open(false), its_samplerate(44100), its_item(0), its_size(0), \
		its_data(nullptr)
	{
		its_good = is_valid_path();
	}

	Ac_outfile_base::Ac_outfile_base(const char *filename):
		Ac_outfile_base(string(filename))
	{}

	Ac_outfile_base::~Ac_outfile_base()
	{
		if (its_data != nullptr)
		{
			delete [] its_data;
		}
	}

	// Get the data to be written, only copy input data, if no prior data
	// has been copied. Only return true if data was copied.
	bool Ac_outfile_base::set_input(double * data, unsigned long int size)
	{
		bool action_done = false;
		if (its_data == nullptr)
		{
			its_data = new double[size];
			its_size = size;

			// Copy data
			for (unsigned long int item = 0;item<its_size;item++)
			{
				its_data[item] = data[item];
			}
			action_done = true; // data was copied
		}
		return action_done;
	}

	// Check if no existing file will be overwritten
	bool Ac_outfile_base::is_valid_path()
	{
		bool is_valid = false; // state to be returned
		path my_path(its_filename); // Create a Boost filesystem path object
		if (exists(my_path) == false)
		{
			is_valid = true;
		}
		else
		{
			its_good = false;
			its_error_msg = its_filename + string(" already exists and will not be overwritten.");
		}

		return is_valid;
	}
} // End of namespace JBS
