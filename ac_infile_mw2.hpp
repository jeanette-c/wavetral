/* ac_infile_mw2.hpp - Declaration of the Microwave II/XT wave dump file input
 * class
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

#ifndef AC_INFILE_MW2_HPP
#define AC_INFILE_MW2_HPP

#include "ac_infile_base.hpp"
#include <fstream>
#include <vector>

namespace JBS {
	// Class to read a waveform from a Waldorf Microwave II/XT SysEx
	class Ac_infile_mw2 : public Ac_infile_base
	{
		public:
			Ac_infile_mw2(const std::string& filename);
			Ac_infile_mw2(const char *filename);
			virtual ~Ac_infile_mw2();

				// File related methods
			virtual bool open();
			virtual void close();
		
				// Data related methods
			virtual bool process();
				// An item here is one waveform, a SysEx file may contain
				// multiple waveforms
			virtual unsigned int get_item_count() { return its_item_count; }
			virtual std::list<std::string> get_item_list();
			virtual bool set_item(unsigned int number);
			static Ac_data_type get_type() { return Ac_data_type::wave; }
			static std::string get_name() { return std::string("Microwave II/XT wave dump"); }
		private:
				// Count items/waveforms in a file and store the waveform numbers
				// internally called by open
			void count_items();
			void read_data(); // read the SysEx string into its_sysex_string
			std::ifstream its_file;
			bool its_ready; // true if file is MW II/XT and has waves
			char *its_sysex_string; // SysEx data as read from the file
			unsigned int its_file_size; // size of the file in bytes
			unsigned int its_item_count;
			std::vector<unsigned int> its_item_numbers; // list of waveforms numbers
			std::vector<unsigned int> its_item_indices; // indices into the SysEx
				// string where each wave begins (first sample)
				// in the file
	}; // End of class Ac_infile_mw2
} // End of namespace JBS

#endif // #ifndef AC_INFILE_MW2_HPP
