/* ac_infile_base.cpp - Definition of the input file base class, not to be
 * instantiated, only as a pointer.
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

#include <boost/filesystem.hpp>
#include "ac_infile_base.hpp"

using std::string;
using std::list;
using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_regular_file;

namespace JBS {
	
	// Constructors
	Ac_infile_base::Ac_infile_base(const string& filename):
		its_filename(filename), its_good(true), its_open(false), \
		its_data(nullptr), its_error_msg(""), its_item(0), its_size(0)
	{
		is_valid_path();
	}

	Ac_infile_base::Ac_infile_base(const char* filename):
		its_filename(filename), its_open(false), its_good(true), \
		its_data(nullptr), its_error_msg(""), its_item(0), its_size(0)
	{
		is_valid_path();
	}

	Ac_infile_base::~Ac_infile_base()
	{
		if (its_data != nullptr)
		{
			delete [] its_data;
			its_data = nullptr;
		}
	}

	list<string> Ac_infile_base::get_item_list()
	{
		list<string> item_list;
		item_list.push_back(string("Default"));
		return item_list;
	}

	// Check whether the filename exists and is a regular file
	bool Ac_infile_base::is_valid_path()
	{
		bool is_valid = false;

		path my_path(its_filename); // Create a Boost filesystem path object
		if (exists(my_path) == true)
		{
			if (is_regular_file(my_path) == true)
			{
				is_valid = true;
			}
			else // not a regular file
			{
				its_good = false;
				its_error_msg = its_filename + string(" is not a file.");
			}
		}
		else // the path does not exist
		{
			its_good = false;
			its_error_msg = its_filename + string(" does not exist.");
		}

		return is_valid;
	}

} // End of namespace JBS
