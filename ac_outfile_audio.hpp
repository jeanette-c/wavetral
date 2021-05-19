/* ac_outfile_audio.hpp - Declaration of the audiofile output class
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

#ifndef AC_OUTFILE_AUDIO_HPP
#define AC_OUTFILE_AUDIO_HPP

#include "ac_outfile_base.hpp"
#include <sndfile.h>

namespace JBS {
	// Class to write audiofiles of waves, only 16bit wav or aiff files
	// are supported.
	class Ac_outfile_audio : public Ac_outfile_base
	{
		public:
			Ac_outfile_audio(const std::string& filename);
			Ac_outfile_audio(const char *filename);
			virtual ~Ac_outfile_audio();

				// File specific methods
			virtual bool open();
			virtual void close();

				// Work related methods
			virtual bool process();
			static Ac_data_type get_type() { return Ac_data_type::wave; }
			static std::string get_name() { return std::string("audio"); }
		private:
			SNDFILE* its_file; // connected audiofile object
			int its_format; // sndfile specific format constant
			SF_INFO its_info; // audiofile info, libsndfile specific
	}; // End of class Ac_outfile_audio
} // End of namespace JBS

#endif // #ifndef AC_OUTFILE_AUDIO_HPP
