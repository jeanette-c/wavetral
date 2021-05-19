/* ac_outfile_mw2.hpp - Declaration of the Microwave II/XT wave dump output
 * file class
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

#ifndef AC_OUTFILE_MW2_HPP
#define AC_OUTFILE_MW2_HPP

#include "ac_outfile_base.hpp"
#include <fstream>

namespace JBS {
	// Class to write a Microwave II/XT waveform (WAVD)
	class Ac_outfile_mw2 : public Ac_outfile_base
	{
		public:
			Ac_outfile_mw2(const std::string& filename);
			Ac_outfile_mw2(const char *filename);
			virtual ~Ac_outfile_mw2();

				// File related methods
			virtual bool open();
			virtual void close();

				// Data related methods
			virtual bool process();
			static Ac_data_type get_type() { return Ac_data_type::harm; }
			static std::string get_name() { return std::string("Microwave II?XT wave dump"); }
		private:
				// Internal method to create the waveform from harmonics
			void make_wave();
			std::ofstream its_file; // connect file to write to
			char *its_sysex_string; // The full SysEx string to be written to file
			bool its_ready; // Set when the wave is prepared
	};
} // End of namespace JBS

#endif // #ifndef AC_OUTFILE_MW2_HPP
