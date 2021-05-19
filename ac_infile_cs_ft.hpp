/* ac_infile_cs_ft.hpp - Declaration of the Csound ftsave-format ftable class
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

#ifndef AC_INFILE_CS_FT_HPP
#define AC_INFILE_CS_FT_HPP

#include <fstream>
#include <string>
#include "ac_infile_base.hpp"

namespace JBS {
	// Class to read files saved by Csound's ftsave or ftsavek opcodes
	class Ac_infile_cs_ft : public Ac_infile_base
	{
		public:
			Ac_infile_cs_ft(const std::string& filename);
			Ac_infile_cs_ft(const char *filename);
			virtual ~Ac_infile_cs_ft();

				// file related methods
			virtual bool open();
			virtual void close();

				// Data related methods
			virtual bool process(); // read and process the input data
			static std::string get_name() { return std::string("Csound ftable"); }
		private:
			std::ifstream its_file; // The actual file object
	}; // End of Ac_infile_cs_ft class
} // end of namespace JBS

#endif // #ifndef AC_INFILE_CS_FT_HPP
