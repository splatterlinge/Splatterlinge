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

#ifndef RESOURCE_AUDIOLOADER_RIFFWAVE_INCLUDED
#define RESOURCE_AUDIOLOADER_RIFFWAVE_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include "audioLoader.h"


/**
 * \addtogroup audioLoader
 *
 * @{
 **/


/// RIFF/Wave audio loader
int audioLoader_riffWave( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format );


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif
