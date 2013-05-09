#include "audioLoader.h"

#include "riffWave.h"
#include "oggVorbis.h"

#include <AL/al.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
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
