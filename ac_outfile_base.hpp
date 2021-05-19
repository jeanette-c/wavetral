/* ac_outfile_base.hpp - Declaration of the output file base class
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

#ifndef AC_OUTFILE_BASE_HPP
#define AC_OUTFILE_BASE_HPP

#include "ac_utility.hpp"
#include <string>

namespace JBS {
	// Base class for output files: interpret the output data/rescale them
	// create the necessary structure around it and write it to a file
	class Ac_outfile_base
	{
		public:
			Ac_outfile_base() = delete;
			Ac_outfile_base(const std::string& filename);
			Ac_outfile_base(const char * filename);
			virtual ~Ac_outfile_base();

				// File realted methods
			virtual bool open() = 0;
			virtual void close() = 0;
			bool is_open() const { return its_open; }
			bool is_good() const { return its_good; }
			const std::string& get_error_msg() const { return its_error_msg; }

				// Work related methods
			virtual bool process() = 0; // format and write data to file
			bool set_input(double* data, unsigned long int size); // supply data
				// Only return true if input data was copied
			unsigned long int get_size() const { return its_size; }
			static Ac_data_type get_type() { return Ac_data_type::both; }
			static std::string get_name() { return std::string(""); }

			/* an item is any one spectrum or waveform, some formats allow
			 * to store multiple items in one file or attach a number to it
			*/
			virtual bool set_item(unsigned int number)
				{
					its_item = number;
					return true;
				}
			unsigned int get_item() const { return its_item; }
			bool set_samplerate(unsigned int samplerate)
				{
					its_samplerate = samplerate;
					return true;
				}
			unsigned int get_samplerate() const { return its_samplerate; }
		protected:
			bool is_valid_path(); // true if file does not exist
			std::string its_filename; // filename to write to
			unsigned int its_item; // optional item number in multi item formats
			unsigned int its_samplerate; // optional samplerate for audio files
			double *its_data; // input data to write
			unsigned long int its_size; // size of data array
			bool its_open; // true if file is open
			bool its_good; // false if an error occured
			std::string its_error_msg; // error message text
	}; // End of class Ac_outfile_base
} // end of namespace JBS

#endif // #ifndef AC_OUTFILE_BASE_HPP
