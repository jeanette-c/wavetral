/* ac_convert_harm_wave.hpp - Class declaration of the harmonic to wave
 * converter
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

#ifndef AC_CONVERT_HARM_WAVE_HPP
#define AC_CONVERT_HARM_WAVE_HPP

#include "ac_convert_base.hpp"

namespace JBS {
	// Class to convert a harmonic spectrum to a waveform
	class Ac_convert_harm_wave : public Ac_convert_base
	{
		public:
			Ac_convert_harm_wave(double zero_threshold, unsigned long int output_size);
			Ac_convert_harm_wave(unsigned long int output_size);
			virtual ~Ac_convert_harm_wave() {}

				// The conversion method
			virtual bool convert();

				// Get input and output type
			static Ac_data_type get_input_type() { return Ac_data_type::harm; }
			static Ac_data_type get_output_type() { return Ac_data_type::wave; }
	}; // End of class Ac_convert_harm_wave
} // End of namespace JBS

#endif // #ifndef AC_CONVERT_HARM_WAVE_HPP
