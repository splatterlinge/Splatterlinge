#include "oggVorbis.h"

#include <AL/al.h>
#include <vorbis/vorbisfile.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


#ifndef RESOURCE_AUDIOLOADER_OGGVORBIS_CHUNKSIZE
#define RESOURCE_AUDIOLOADER_OGGVORBIS_CHUNKSIZE (4096 * 2)
#endif


int audioLoader_oggVorbis( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format )
{

	FILE * file = fopen( filename, "rb" );
	if( !file )
		return -ENOENT;

	OggVorbis_File oggVorbisFile;
	if( ov_open( file, &oggVorbisFile, NULL, 0 ) < 0 )
	{
		ov_clear( &oggVorbisFile );
		return -EPROTONOSUPPORT;
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
		return -EPROTONOSUPPORT;
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
