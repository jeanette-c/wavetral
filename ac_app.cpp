/* ac_app.cpp - The main app class definition
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

#include "ac_app.hpp"
#include "config.h"
#include <exception>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>
using std::cout;
using std::endl;
using std::exception;

using std::string;

namespace JBS {
	// Set the app name
	string Ac_app::its_app_name = string("Wavetral");
	Ac_app::Ac_app(int argc, char *argv[]):
		its_good(true), its_error_msg(""), its_input_item(0), its_output_item(0), \
		its_output_samplerate(44100), its_input_filename(""), \
		its_output_filename(""), its_convert_threshold(1e-5), its_output_size(0), \
		its_infile(nullptr), its_converter(nullptr), its_outfile(nullptr), \
		its_done(false)
	{
		its_good = parse_options(argc, argv); // parse options
	} // End of constructor

	Ac_app::~Ac_app()
	{
		// Close and delete output file
		if (its_outfile != nullptr)
		{
			its_outfile->close();
			delete its_outfile;
		}

		// Delete converter
		if (its_converter != nullptr)
		{
			delete its_converter;
		}

		// Close, delete input file
		if (its_infile != nullptr)
		{
			its_infile->close();
			delete its_infile;
		}
	} // Deconstructor

	// Print a version message
	void Ac_app::version()
	{
		cout << PACKAGE_STRING << "\nCopyright (c) 2021 Jeanette C., <jeanette@juliencoder.de>\nThis is free software, licensed under the GNU General Public License (GPL)\nversion 3 or later.\n";
	} // End of version

	// Detect input/output module by filename extensions
	string Ac_app::detect_format(const string& filename)
	{
		// Get the filename extension by searching for "." from the end
		size_t extension_pos = filename.rfind(".");
		string extension = filename.substr(extension_pos + 1);
		string format = "";
		
		// Check for all recognised extensions
		if ((extension.compare("wav") == 0) || (extension.compare("aiff") == 0))
		{
			format = string("audio");
		}
		else if (extension.compare("txt") == 0)
		{
			format = string("text");
		}
		else if (extension.compare("ftable") == 0)
		{
			format = string("ftable");
		}
		else if (extension.compare("syx") == 0)
		{
			format = string("mw2");
		}
		else if (extension.compare("ysc") == 0)
		{
			format = string("ysc");
		}
		else
		{
			format = string("none");
		}

		return format;
	} // End of detect_format

	// List items from the input file
	void Ac_app::list_items(string& type)
	{
		if (its_infile->open() == true)
		{
			cout << its_app_name << ": listing ";
			if (type.compare("harm") == 0)
			{
				cout << "spectra";
			}
			else
			{
				cout << "waveforms";
			}
			cout << " from file " << its_input_filename << ":\n";
			for (auto item: its_infile->get_item_list())
			{
				cout << item << endl;
			}
			its_infile->close();
		}
		else // file couldn\t be opened
		{
			its_good = false;
			its_error_msg = its_infile->get_error_msg();
		}
	} // End of list_items

	// Create input file object
	bool Ac_app::make_input(string& input_format, string& input_type)
	{
		// Check if there is a format, if not try to detect one
		if (input_format.empty())
		{
			input_format = detect_format(its_input_filename);
		}

		// Check if the input format is valid
		if (input_format.compare("audio") == 0)
		{
			its_infile = new Ac_infile_audio(its_input_filename);
			input_type = string("wave");
		}
		else if (input_format.compare("text") == 0)
		{
			its_infile = new Ac_infile_plaintext(its_input_filename);
		}
		else if (input_format.compare("ftable") == 0)
		{
			its_infile = new Ac_infile_cs_ft(its_input_filename);
		}
		else if (input_format.compare("mw2") == 0)
		{
			its_infile = new Ac_infile_mw2(its_input_filename);
			input_type = string("wave");
		}
		else
		{
			its_good = false;
			its_done = true;
			its_error_msg = its_input_filename + string(" is not recognised as a supported format: ") + input_format;
			return false;
		}

		// Check if a type is set
		if ((input_format.compare("text") == 0) || (input_format.compare("ftable") == 0))
		{
			if (input_type.empty())
			{
				its_good = false;
				its_done = true;
				its_error_msg = string("No type was given for the input file ") + its_input_filename + string("\nBut input format ") + input_format + string(" supports both wave and harm. Please specify.");
				return false;
			}
		}

		return true;
	}

	// Create output file object
	bool Ac_app::make_output(string& output_format, string& output_type)
	{
		// Check for format, if none present, try to detect one
		if (output_format.empty())
		{
			output_format = detect_format(its_output_filename);
		}
				
		if (output_format.compare("text") == 0)
		{
			its_outfile = new Ac_outfile_plaintext(its_output_filename);
		}
		else if (output_format.compare("audio") == 0)
		{
			its_outfile = new Ac_outfile_audio(its_output_filename);
			output_type = string("wave");
		}
		else if (output_format.compare("mw2") == 0)
		{
			its_outfile = new Ac_outfile_mw2(its_output_filename);
			output_type = string("harm");
			its_output_size = 64;
			if (its_output_item == 0)
			{
				its_output_item = 1000;
			}
		}
		else if (output_format.compare("ysc") == 0)
		{
			its_outfile = new Ac_outfile_yoshimi(its_output_filename);
			output_type = string("harm");
		}
		else // unrecognised format
		{
			its_good = false;
			its_done = true;
			its_error_msg = its_output_filename + string(" is not recognised as a supported format: ") + output_format;
			return false;
		}

		// Check if a type is set
		if (output_format.compare("text") == 0)
		{
			if (output_type.empty())
			{
				its_good = false;
				its_done = true;
				its_error_msg = string("No type was given for the output file ") + its_output_filename + string("\nBut the output format ") + output_format + string(" supports both wave and harm. Please specify.");
				return false;
			}
		}
		return true;
	}

	// Parse options and possibly create input, converter and output
	bool Ac_app::parse_options(int argc, char *argv[])
	{
			// Create necessary temporary variables
		string input_format = ""; // file format
		string output_format = ""; // file format
		string input_type = ""; // data type: wave or harm
		string output_type = ""; // data type: wave or harm

		try
		{
			// Set up all commandline options
			po::options_description help_desc("Help options");
			help_desc.add_options()
				("help,h", "Show this help")
				("version,v", "Show version information")
				("list,l", po::value<string>()->value_name("input_file"), "List available waves from the input file, only necessary for mw2 files with multiple waveforms.")
			;
			po::options_description conv_desc("Conversion options");
			conv_desc.add_options()
				("input,i", po::value<string>()->value_name("input_file"), "Input file to be converted")
				("output,o", po::value<string>()->value_name("output_file"), "Output file to be written to")
				("input_format,f", po::value<string>()->value_name("input_format"), "Input file format: audio (wav and aiff), text (one value per line), ftable (for Csound ftsave(k) generated files), mw2 (Waldorf Microwave II/XT wave dump)")
				("output_format,F", po::value<string>()->value_name("output_format"), "Output file format: audio (wav and aiff), text (one value per line), ysc (Yoshimi script), mw2 (Waldorf Microwave II/XT wave dump)")
				("input_type,t", po::value<string>()->value_name("input_type"), "Type of the input data, wave or harm, only necessary for text and ftable files, since they can store both.")
				("output_type,T", po::value<string>()->value_name("output_type"), "Type of the output data, wave or harm, only necessary for text and ftable files, since they can store both.")
				("samplerate,r", po::value<unsigned int>()->value_name("samplerate"), "Samplerate of the output file, only necessary for audio output (default 44100)")
				("output_size,s", po::value<unsigned long int>()->value_name("output_size"), "number of sample or harmonics for the output")
				("input_wave,w", po::value<unsigned int>()->value_name("input_wave_number"), "Number of the input wave, only necessary for mw2 files with multiple waves. List all waves with the --list option. (default: 0)")
				("output_wave,W", po::value<unsigned int>()->value_name("output_wavenumber"), "Output wave number, only necessary for mw2 output. Valid numbers range from 1000 to 1249 (default: 1000)")
				("threshold,z", po::value<double>()->value_name("zero_threshold"), "Value below which harmonics will be set to zero (default: 1 * 10^(-5))")
			;
			po::options_description commandline_desc;
			commandline_desc.add(help_desc).add(conv_desc);
			po::variables_map cmd_options;
			store(po::parse_command_line(argc,argv,commandline_desc), cmd_options);
			notify(cmd_options);

			if (cmd_options.size() == 0)
			{
				its_good = false;
				its_done = true;
				its_error_msg = its_app_name + string(" - convert between single cycle waveforms and spectra.\nNo options given try:\n") + string(argv[0]) + string(" -h to get help.");
				return false;
			}

			// Process used options
			if (cmd_options.count("help"))
			{
				cout << PACKAGE_STRING << endl;
				cout << "Copyright (c) 2021 Jeanette C., <jeanette@juliencoder.de>\n";
				cout << "Released under the GPL version 3.\n";
				cout << its_app_name << " convert between single cycle waveforms and spectra in different formats.\n\n";
				cout << commandline_desc << endl;
				its_done = true;
				return true;
			}

			if (cmd_options.count("version"))
			{
				version();
				its_done = true;
				return true;
			}

			if (cmd_options.count("input_format"))
			{
				input_format = cmd_options["input_format"].as<string>();
			}
			if (cmd_options.count("output_format"))
			{
				output_format = cmd_options["output_format"].as<string>();
			}
			if (cmd_options.count("input_type"))
			{
				input_type = cmd_options["input_type"].as<string>();
			}
			if (cmd_options.count("output_type"))
			{
				output_type = cmd_options["output_type"].as<string>();
			}

			if (cmd_options.count("list"))
			{
				its_input_filename = cmd_options["list"].as<string>();
				if (make_input(input_format, input_type) == true)
				{
					list_items(input_type);
					its_done = true;
					return its_good;
				}
				else
				{
					its_done = true;
					return its_good;
				}
			}
	
			if (cmd_options.count("input"))
			{
				its_input_filename = cmd_options["input"].as<string>();
			}
			else
			{
				its_good = false;
				its_done = true;
				its_error_msg = string("No input file given. Please specify an input file.");
				return false;
			}

			if (cmd_options.count("output"))
			{
				its_output_filename = cmd_options["output"].as<string>();
			}
			else
			{
				its_good = false;
				its_done = true;
				its_error_msg = string("No output file given. Please specify an output file.");
				return false;
			}

			if (cmd_options.count("samplerate"))
			{
				its_output_samplerate = cmd_options["samplerate"].as<unsigned int>();
			}
			if (cmd_options.count("output_size"))
			{
				its_output_size = cmd_options["output_size"].as<unsigned long int>();
			}

			if (cmd_options.count("input_wave"))
			{
				its_input_item = cmd_options["input_wave"].as<unsigned int>();
			}
			if (cmd_options.count("output_wave"))
			{
				its_output_item = cmd_options["output_wave"].as<unsigned int>();
			}
			if (cmd_options.count("threshold"))
			{
				its_convert_threshold = cmd_options["threshold"].as<double>();
			}
		} // end of try
		catch(exception &e)
		{
			cout << "ERROR parsing options:\n" << e.what() << endl;
			its_good = false;
			its_done = true;
			its_error_msg = string("Failed to parse the program options.");
		}

		// If operation is complete, return health state
		if (its_done == true)
		{
			return its_good;
		}

		// Now check plausibility of options and create objects
		// Create input file object
		if (make_input(input_format,input_type) == false)
		{
			return false;
		}

		// Create the output file object
		if (make_output(output_format, output_type) == false)
		{
			return false;
		}

		// Now check the output size, if 0 return an error
		if (its_output_size == 0)
		{
			its_good = false;
			its_done = true;
			its_error_msg = string("No output size given. Please specify an output size.");
			return false;
		}

			// Create the converter, based on input and output type
			// First check for an output_size
		if ((input_type.compare("harm") == 0) && (output_type.compare("harm") == 0))
		{
			its_converter = new Ac_convert_harm_harm(its_convert_threshold, its_output_size);
		}
		else if ((input_type.compare("harm") == 0) && (output_type.compare("wave") == 0))
		{
			its_converter = new Ac_convert_harm_wave(its_convert_threshold, its_output_size);
		}
		else if ((input_type.compare("wave") == 0) && (output_type.compare("wave") == 0))
		{
			its_converter = new Ac_convert_wave_wave(its_convert_threshold, its_output_size);
		}
		else if ((input_type.compare("wave") == 0) && (output_type.compare("harm") == 0))
		{
			its_converter = new Ac_convert_wave_harm(its_convert_threshold, its_output_size);
		}
		else // no conversion between input and output type?
		{
			its_good = false;
			its_done = true;
			its_error_msg = string("Can't convert between input type ") + input_type + string(" and output type ") + output_type;
			return false;
		}

		return true;
	} // End of parse_options

	// convert input to output
	bool Ac_app::convert()
	{
		// Assume that input, converter have been created and configured
		its_converter->set_input(its_infile->get_data(), its_infile->get_size());
		if (its_converter->convert() == true)
		{
			return true;
		}
		else
		{
			its_good = false;
			its_error_msg = its_converter->get_error_msg();
			return false;
		}

		return true;
	}

	// Run the main program
	bool Ac_app::run()
	{
		// Check if there is nothing more to do
		if (its_done == true)
		{
			if (its_good == false)
			{
				cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
				return false;
			}
			else
			{
				return true;
			}
		}

		// Check if an input file object was created
		if (its_infile == nullptr)
		{
			if (its_error_msg.empty())
			{
				its_error_msg = string("Could not create input file ") + its_input_filename;
			}
			its_good = false;
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Set up the input file
		if (its_infile->set_item(its_input_item) == false)
		{
			its_good = false;
			its_error_msg = its_infile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Open the input file
		if (its_infile->open() == false)
		{
			its_good = false;
			its_error_msg = its_infile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Check output file object
		if (its_outfile == nullptr)
		{
			its_good = false;
			if (its_error_msg.empty())
			{
				its_error_msg = string("Could not open outputfile ") + its_output_filename;
			}
			its_good = false;
			cout<< its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Configure output file
		// set item
		if (its_outfile->set_samplerate(its_output_samplerate) == false)
		{
			its_good = false;
			its_error_msg = its_outfile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}
		
		// Set item
		if (its_outfile->set_item(its_output_item) == false)
		{
			its_good = false;
			its_error_msg = its_outfile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Open the connected file
		if (its_outfile->open() == false)
		{
			its_good = false;
			its_error_msg = its_outfile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Read input
		if (its_infile->process() == false)
		{
			its_good = false;
			its_error_msg = its_infile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Pass input data to the converter
		its_converter->set_input(its_infile->get_data(), its_infile->get_size());
		if (its_converter->is_good() == false)
		{
			its_good = false;
			its_error_msg = its_converter->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Print output samplerate
		cout << "Converting to " << its_outfile->get_samplerate() << "Hz\n";

		// Convert input data
		if (its_converter->convert() == false)
		{
			its_good = false;
			its_error_msg = its_converter->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// if converter has no output fail
		if (its_converter->is_ready() == false)
		{
			its_good = false;
			its_error_msg = its_converter->get_error_msg();
			if (its_error_msg.empty())
			{
				its_error_msg = string("Conversion failed: no output was produced.");
			}
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Write data to output file
		if (its_outfile->set_input(its_converter->get_output(), its_converter->get_size()) == false)
		{
			its_good = false;
			its_error_msg = its_outfile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Process/write output data to file
		if (its_outfile->process() == false)
		{
			its_good = false;
			its_error_msg = its_outfile->get_error_msg();
			cout << its_app_name << " ERROR:\n" << its_error_msg << endl;
			return false;
		}

		// Conversion is complete, print an informative message
		cout << "Converted ";
		if (its_converter->get_input_type() == Ac_data_type::harm)
		{
			cout << "Spectrum ";
		}
		else
		{
			cout << "Wave ";
		}
		cout << "from " << its_infile->get_name() << " file " << its_input_filename << endl;
		cout << "to ";
		if (its_converter->get_output_type() == Ac_data_type::harm)
		{
			cout << "spectrum ";
		}
		else
		{
			cout << "wave ";
		}
		cout << its_outfile->get_name() << " file " << its_output_filename << " with " << its_outfile->get_size();
		if (its_converter->get_output_type() == Ac_data_type::wave)
		{
			cout << " harmonics.";
		}
		else
		{
			cout << " samples.";
		}
		cout << endl;
		
		return true;
	}
} // End of namespace JBS
