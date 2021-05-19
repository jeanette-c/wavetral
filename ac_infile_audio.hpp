/* ac_infile_audio.hpp - Declaration of the audio input file class
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

#ifndef AC_INFILE_AUDIO_HPP
#define AC_INFILE_AUDIO_HPP

#include <string>
#include <sndfile.h>
#include "ac_infile_base.hpp"

namespace JBS {
	// Class to read waveform from audiofiles using libsndfile
	class Ac_infile_audio : public Ac_infile_base
	{
		public:
			Ac_infile_audio(const std::string& filename);
			Ac_infile_audio(const char * filename);
			~Ac_infile_audio();

				// File realted methods
			virtual bool open();
			virtual void close();

				// Data related methods
			virtual bool process();
			static Ac_data_type get_type() { return Ac_data_type::wave; }
			static std::string get_name() { return std::string("audio"); }
		private:
			SNDFILE *its_file; // audio file object
			SF_INFO its_info; // soundfile info supplied by libsndfile
	}; // End of class Ac_infile_audio
} // End of namespace JBS

#endif // #ifndef AC_INFILE_AUDIO_HPP
