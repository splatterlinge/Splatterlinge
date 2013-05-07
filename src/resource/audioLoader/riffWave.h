#ifndef RESOURCE_AUDIOLOADER_RIFFWAVE_INCLUDED
#define RESOURCE_AUDIOLOADER_RIFFWAVE_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include <AL/al.h>


int audioLoader_riffWave( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format );


#ifdef __cplusplus
}
#endif

#endif
