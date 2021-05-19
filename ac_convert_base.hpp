/* ac_convert_base.hpp - Declaration of the base converter class, not to be
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

#ifndef AC_CONVERT_BASE_HPP
#define AC_CONVERT_BASE_HPP

#include "ac_utility.hpp"
#include <string>

namespace JBS {
	// Base class for all converters
	class Ac_convert_base
	{
		public:
			Ac_convert_base() = delete;
			Ac_convert_base(double zero_threshold, unsigned long int output_size);
			Ac_convert_base(unsigned long int output_size);
			virtual ~Ac_convert_base();

				// Data related methods
			bool is_ready() const { return its_ready; }
			bool is_good() const { return its_good; }
			const std::string& get_error_msg() const { return its_error_msg; }
			void set_size(unsigned long int new_size);
			unsigned long int get_size() const { return its_size; }
			unsigned long int get_src_size() const { return its_src_size; }
			static Ac_data_type get_input_type() { return Ac_data_type::both; }
			static Ac_data_type get_output_type() { return Ac_data_type::both; }
			double* get_output();
			void set_input(double *input, unsigned long int size);
			virtual bool convert() = 0; // the actual converter method
		protected:
			unsigned long int its_size; // desired output size/number of samples
			unsigned long int its_src_size; // size of input
			double its_zero_threshold; // threshold below which values will be
				// changed to zero
			double *its_input; // input data to be converted
			double *its_output; // converted data
			bool its_ready; // true if output is ready
			bool its_good; // true as long as there are no errors
			std::string its_error_msg; // error message text
	}; // End of class Ac_convert_base
} // end of namespace JBS

#endif // #ifndef AC_CONVERT_BASE_HPP
