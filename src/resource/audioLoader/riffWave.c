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

#include "riffWave.h"

#include <AL/al.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/// The Resource Interchange File Format (RIFF) container header
typedef struct
{
	char chunkID[4];	// should contain "RIFF"
	uint32_t chunkSize;	// excluding chunkID and chunkSize
	char format[4];		// should contain "WAVE"
} RIFFHeader;


/// Wave format descriptor
typedef struct
{
	char subChunkID[4];	// should contain "fmt "
	uint32_t subChunkSize;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
} WAVEFormat;


/// Wave data header
typedef struct
{
	char subChunkID[4];	// should contain "data"
	uint32_t subChunkSize;
} WAVEDataHeader;


int audioLoader_riffWave( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format )
{
	FILE * file = fopen( filename, "rb" );
	if( !file )
		return -AUDIOLOADER_FILENOTFOUND;

	////////////////////////////////
	// RIFF header
	RIFFHeader riffHeader;
	if( !fread( &riffHeader, sizeof(RIFFHeader), 1, file ) )
	{
		fclose( file );
		return -AUDIOLOADER_INVALIDFORMAT;
	}
	if( strncmp( riffHeader.chunkID, "RIFF", 4 ) )
	{
		fclose( file );
		return -AUDIOLOADER_INVALIDCONTAINER;
	}
	if( strncmp( riffHeader.format, "WAVE", 4 ) )
	{
		fclose( file );
		return -AUDIOLOADER_INVALIDCODEC;
	}
	////////////////////////////////

	////////////////////////////////
	// WAVE format header
	WAVEFormat waveFormat;
	if( !fread( &waveFormat, sizeof(WAVEFormat), 1, file ) )
	{
		fclose( file );
		return -AUDIOLOADER_INVALIDFORMAT;
	}
	if( strncmp( waveFormat.subChunkID, "fmt ", 4 ) )
	{
		fclose( file );
		return -AUDIOLOADER_INVALIDFORMAT;
	}
	// skip extra parameters
	size_t extraParameterSize = waveFormat.subChunkSize - 16;
	if( extraParameterSize > 0 )
		fseek( file, extraParameterSize, SEEK_CUR );
	////////////////////////////////

	////////////////////////////////
	// WAVE data header
	WAVEDataHeader waveDataHeader;
	if( !fread( &waveDataHeader, sizeof(WAVEDataHeader), 1, file ) )
	{
		fclose( file );
		return -AUDIOLOADER_INVALIDFORMAT;
	}
	if( strncmp( waveDataHeader.subChunkID, "data", 4 ) )
	{
		fclose( file );
		return -AUDIOLOADER_INVALIDFORMAT;
	}
	////////////////////////////////

	////////////////////////////////
	// data
	unsigned char * data = malloc( waveDataHeader.subChunkSize );
	if( !fread( data, waveDataHeader.subChunkSize, 1, file ) )
	{
		fclose( file );
		free( data );
		return -AUDIOLOADER_INVALIDFORMAT;
	}
	fclose( file );
	////////////////////////////////

	*frequency = waveFormat.sampleRate;

	*format = 0;
	switch( waveFormat.numChannels )
	{
	case 1:
		switch( waveFormat.bitsPerSample )
		{
		case 8:		*format = AL_FORMAT_MONO8;	break;
		case 16:	*format = AL_FORMAT_MONO16;	break;
		}
		break;
	case 2:
		switch( waveFormat.bitsPerSample )
		{
		case 8:		*format = AL_FORMAT_STEREO8;	break;
		case 16:	*format = AL_FORMAT_STEREO16;	break;
		}
		break;
	}
	if( !*format )
	{
		free( data );
		return -AUDIOLOADER_INVALIDFORMAT;
	}

	alGenBuffers( 1, buffer );
	alBufferData( *buffer, *format, (void*)data, waveDataHeader.subChunkSize, *frequency );
	free( data );
	return 0;
}
