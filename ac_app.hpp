/* ac_app.hpp - Declaration of the main app class
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

#ifndef AC_APP_HPP
#define AC_APP_HPP

#include "ac_infile.hpp"
#include "ac_convert.hpp"
#include "ac_outfile.hpp"
#include <string>

namespace JBS {
	// Class to run the whole conversion process
	// process options: check input, output converter compatibility and run
	class Ac_app
	{
		public:
			Ac_app(int argc, char *argv[]);
			~Ac_app();

				// Run the procedure
			bool run();

				// State related methods
				bool is_good() const { return its_good; }
			std::string get_error_msg() const { return its_error_msg; }
		private:
				// Detect format by filename extension
			std::string detect_format(const std::string& filename);
				// Create input file object
			bool make_input(std::string& input_format, std::string& input_type);
				// Create output file object
			bool make_output(std::string& output_format, std::string& output_type);
				// Parse options and create objects
			bool parse_options(int argc, char *argv[]);
				// Run the conversion process
			bool convert();
				// List items
			void list_items(std::string& type);
				// Print version information
			void version();

				// Private data memebers
			Ac_infile_base *its_infile; // The input file to be read
			Ac_convert_base *its_converter; // Actual data converter
			Ac_outfile_base *its_outfile; // Output to be written
			bool its_good; // general health of this object
			std::string its_error_msg; // Error message text of this object
			bool its_done; // true if no task is left (used by help and version print
			
				// Work specific variables
			unsigned int its_input_item; // wave/item from the input file
			unsigned int its_output_item; // waveform;/item to be written to output
			unsigned int its_output_samplerate; // samplerate, only for audio outputs
			std::string its_input_filename;
			std::string its_output_filename;
			double its_convert_threshold; // zero threshold for converters
			unsigned long int its_output_size; // desired size of the output
			static std::string its_app_name; // name of the program
	}; // End of class Ac_app
} // End of namespace JBS

#endif // #ifndef AC_APP_HPP
