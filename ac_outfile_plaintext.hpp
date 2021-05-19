/* ac_outfile_plaintext.hpp - Declaration of the plaintext output file class
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

#ifndef AC_OUTFILE_PLAINTEXT_HPP
#define AC_OUTFILE_PLAINTEXT_HPP

#include "ac_outfile_base.hpp"
#include <fstream>

namespace JBS {
	// Write plaintext data, one double value per line
	class Ac_outfile_plaintext : public Ac_outfile_base
	{
		public:
			Ac_outfile_plaintext(const std::string& filename);
			Ac_outfile_plaintext(const char *filename);
			virtual ~Ac_outfile_plaintext();

				// File related methods
			virtual bool open();
			virtual void close();

				// Work related methods
			virtual bool process(); // format and write the data
			static std::string get_name() { return std::string("text"); }
		private:
			std::ofstream its_file; // connected file object for writing
	}; // End of class Ac_outfile_plaintext
} // End of namespace JBS

#endif // #ifndef AC_OUTFILE_PLAINTEXT_HPP
