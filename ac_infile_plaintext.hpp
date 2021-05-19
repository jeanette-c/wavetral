/* ac_infile_plaintext.hpp - Declaration of the plaintext input file class
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

#ifndef AC_INFILE_PLAINTEXT_HPP
#define AC_INFILE_PLAINTEXT_HPP

#include <fstream>
#include "ac_infile_base.hpp"

namespace JBS {
	// Class to read Csound GEN23 ready files, i.e. text files wioth one
	// harmonic value per line
	class Ac_infile_plaintext : public Ac_infile_base
	{
		public:
			Ac_infile_plaintext(const std::string& filename);
			Ac_infile_plaintext(const char * filename);
			virtual ~Ac_infile_plaintext();

				// File related methods
			virtual bool open();
			virtual void close();

				// Data related methods
			virtual bool process(); // Read harmonics
			static std::string get_name() { return std::string("text"); }
		private:
			std::ifstream its_file; // the actual file object to use
	}; // End of class Ac_infile_plaintext
} // End of namespace JBS

#endif // #ifndef AC_INFILE_PLAINTEXT_HPP
