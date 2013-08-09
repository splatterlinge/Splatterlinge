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

#include "audioLoader.h"

#include "riffWave.h"
#include "oggVorbis.h"

#include <AL/al.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int audioLoader( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format )
{
	int ret = 0;

	ret = audioLoader_riffWave( filename, buffer, frequency, format );
	if( ret == 0 )
		return ret;

	ret = audioLoader_oggVorbis( filename, buffer, frequency, format );
	return ret;
}
