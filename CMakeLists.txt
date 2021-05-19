# This is the CMakeLists.txt (cmake system build file) for wavetral
# written by Jeanette C.
# This is free software distributed under the terms of the GNU General Public
# License version 3. See the file COPYING for further information on the GPL.
# Set up CMake
cmake_minimum_required (VERSION 3.1)
# Include additional find modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# Enable c++11 standard
set (CMAKE_CXX_STANDARD 11)
project (wavetral C CXX) # project name and involved programming languages
# The main executable and its source files
add_executable (wavetral main.cpp ac_utility.cpp ac_infile_base.cpp ac_infile_audio.cpp ac_infile_cs_ft.cpp ac_infile_mw2.cpp ac_infile_plaintext.cpp ac_outfile_base.cpp ac_outfile_audio.cpp ac_outfile_mw2.cpp ac_outfile_plaintext.cpp ac_outfile_yoshimi.cpp ac_convert_base.cpp ac_convert_harm_harm.cpp ac_convert_harm_wave.cpp ac_convert_wave_harm.cpp ac_convert_wave_wave.cpp ac_app.cpp)

# Include current dir and binary
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (INCS ${INCS} ${PROJECT_BINARY_DIR})

# find libboost_program_options and libboost_filesystem
set(Boost_USE_MULTITHREADED ON) # use multithreaded versions
if (CMAKE_SYSTEM_NAME MATCHES "Darwin") # if this is Mac OS
	set(Boost_USE_STATIC_LIBS ON) # for MAC OS
	set(Boost_USE_STATIC_RUNTIME ON) # for MAC OS
endif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
find_package(Boost 1.58 REQUIRED COMPONENTS program_options filesystem system)
if (Boost_PROGRAM_OPTIONS_FOUND)
	message(STATUS "found libboost_program_options")
	set (INCS ${INCS} ${Boost_INCLUDE_DIRS})
	set (LIBS ${LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY})
else (Boost_PROGRAM_OPTIONS_FOUND)
	message(FATAL_ERROR "libboost_program_options is required but not found")
endif (Boost_PROGRAM_OPTIONS_FOUND)

if (Boost_FILESYSTEM_FOUND)
	message(STATUS "Found libboost_filesystem")
	set(LIBS ${LIBS} ${Boost_FILESYSTEM_LIBRARY})
else (Boost_FILESYSTEM_FOUND)
	message(FATAL_ERROR "libboost_filesystem is required but not found")
endif (Boost_FILESYSTEM_FOUND)

# Find fftw3, for that we need pkgconfig
find_package (PkgConfig REQUIRED)

pkg_check_modules(FFTW3 REQUIRED fftw3)
if (FFTW3_FOUND)
	message(STATUS "Found libfftw3")
	set (INCS ${INCS} ${FFTW3_INCLUDE_DIRS})
	set (LIBS ${LIBS} ${FFTW3_LIBRARIES})
else (FFTW3_FOUND)
	message(FATAL_ERROR "libfftw3 not found, but required")
endif (FFTW3_FOUND)

find_package(LibSndFile REQUIRED)
if (LIBSNDFILE_FOUND)
	message(STATUS "Found libsndfile")
	set (INCS ${INCS} ${LIBSNDFILE_INCLUDE_DIRS})
	set (LIBS ${LIBS} ${LIBSNDFILE_LIBRARIES})
else (LIBSNDFILE_FOUND)
	message(FATAL_ERROR "libsndfile not found, but required")
endif (LIBSNDFILE_FOUND)

find_package(LibSamplerate REQUIRED)
if (LIBSAMPLERATE_FOUND)
	message(STATUS "Found libsamplerate")
	set (INCS ${INCS} ${LIBSAMPLERATE_INCLUDE_DIR})
	set (LIBS ${LIBS} ${LIBSAMPLERATE_LIBRARY})
else (LIBSAMPLERATE_FOUND)
	message(FATAL_ERROR "libsamplerate not found, but required")
endif (LIBSAMPLERATE_FOUND)

# Complete the set up
include_directories (${INCS})
target_link_libraries (wavetral ${LIBS})
install (TARGETS wavetral DESTINATION bin)
install (FILES wavetral.1 DESTINATION man/man1)
