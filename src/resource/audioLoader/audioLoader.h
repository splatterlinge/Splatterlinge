/*
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RESOURCE_AUDIOLOADER_INCLUDED
#define RESOURCE_AUDIOLOADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include <AL/al.h>

/**
 * \defgroup audioLoader Audio Loader
 * \brief Loader for different audio file formats
 *
 * @{
 **/


#define AUDIOLOADER_NOERROR		0	///< No error occurred
#define AUDIOLOADER_FILENOTFOUND	1	///< File not existing
#define AUDIOLOADER_INVALIDCONTAINER	2	///< Unsupported container
#define AUDIOLOADER_INVALIDCODEC	3	///< Unsupported codec
#define AUDIOLOADER_INVALIDFORMAT	4	///< Unsupported format


/// Loads the given audio file to a new OpenAL buffer and returns its identifier and additional information
/**
 * The loader tries to load the file with all implemented loaders until one loader succeeds.
 * @param filename The audio file to load
 * @param buffer Pointer for returning an OpenAL buffer identifier - only valid if return value is true
 * @param frequency Pointer for returning the sample's frequency - only valid if return value is true
 * @param format Pointer for returning the sample's format - only valid if return value is true
 */
int audioLoader( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format );


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
