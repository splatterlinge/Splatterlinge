/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
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

#include "oggVorbis.h"

#include <AL/al.h>
#include <vorbis/vorbisfile.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifndef RESOURCE_AUDIOLOADER_OGGVORBIS_CHUNKSIZE
#define RESOURCE_AUDIOLOADER_OGGVORBIS_CHUNKSIZE (4096 * 2)
#endif


int audioLoader_oggVorbis( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format )
{

	FILE * file = fopen( filename, "rb" );
	if( !file )
		return -AUDIOLOADER_FILENOTFOUND;

	OggVorbis_File oggVorbisFile;
	if( ov_open( file, &oggVorbisFile, NULL, 0 ) < 0 )
	{
		ov_clear( &oggVorbisFile );
		return -AUDIOLOADER_INVALIDCONTAINER;
	}

	vorbis_info * vorbisInfo = ov_info( &oggVorbisFile, -1 );

	*format = 0;
	switch( vorbisInfo->channels )
	{
		case 1:
			*format = AL_FORMAT_MONO16;
			break;
		case 2:
			*format = AL_FORMAT_STEREO16;
			break;
	}
	if( !*format )
	{
		ov_clear( &oggVorbisFile );
		return -AUDIOLOADER_INVALIDFORMAT;
	}

	*frequency = vorbisInfo->rate;

	int bitStream = 0;
	long bytesRead = 0;
	char * data = 0;
	long dataSize = 0;
	do {
		data = realloc( data, dataSize + RESOURCE_AUDIOLOADER_OGGVORBIS_CHUNKSIZE );
		bytesRead = ov_read( &oggVorbisFile, data + dataSize, RESOURCE_AUDIOLOADER_OGGVORBIS_CHUNKSIZE, 0, 2, 1, &bitStream );
		dataSize += bytesRead;
	} while( bytesRead > 0 );
	data = realloc( data, dataSize );

	ov_clear( &oggVorbisFile );

	alGenBuffers( 1, buffer );
	alBufferData( *buffer, *format, data, dataSize, *frequency );

	free( data );
	return 0;
}
