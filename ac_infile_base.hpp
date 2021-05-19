/* ac_infile_base.hpp - Declaration of the input file base class
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

#ifndef AC_INFILE_BASE_HPP
#define AC_INFILE_BASE_HPP

#include "ac_utility.hpp"
#include <string>
#include <list>

namespace JBS {
	// Abstract base class for file input
	class Ac_infile_base
	{
		public:
				// Constructors and destructor
			Ac_infile_base() = delete; // no default constructor
			Ac_infile_base(const std::string& filename);
			Ac_infile_base(const char* filename);
			virtual ~Ac_infile_base();

				// Accessros, state information
				// File states
			bool is_open() const { return its_open; }
			bool is_good() const { return its_good; }
			const std::string& get_error_msg() const { return its_error_msg; }
			static Ac_data_type get_type() { return Ac_data_type::both; }
			static std::string get_name() { return std::string(""); }
			virtual void close() = 0;
			virtual bool open() = 0;

				// Data related methods
			virtual bool process() = 0; // read and convert the data to double
			double* get_data() { return its_data; }
			unsigned long int get_size() const { return its_size; }

				/* An item is any one spectrum or wave, some formats allow for
			 		multiple "items" in one file.
					This "default" implementation assume one item
				*/
			virtual unsigned int get_item_count() { return 1; }
			virtual std::list<std::string> get_item_list();
			virtual bool set_item(unsigned int number) { return true; }
		protected:
			bool is_valid_path(); // check whether filename exists and is a file
			std::string its_filename; // filename of the input file
			std::string its_error_msg; // error message string
			bool its_open; // true if connected file is open
			bool its_good; // true if there are no errors
			unsigned int its_item;
			double *its_data; // data read from the file
			unsigned long int its_size; // elements in the data array
	}; // End of Ac_infile_base class

} // End of namespace JBS

#endif // #ifndef AC_INFILE_BASE_HPP
