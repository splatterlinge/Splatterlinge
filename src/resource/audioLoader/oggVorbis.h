#ifndef RESOURCE_AUDIOLOADER_OGGVORBIS_INCLUDED
#define RESOURCE_AUDIOLOADER_OGGVORBIS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include "audioLoader.h"


/**
 * \addtogroup audioLoader
 *
 * @{
 **/


/// OGG-Vorbis audio loader
int audioLoader_oggVorbis( const char * filename, ALuint * buffer, ALsizei * frequency, ALenum * format );


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif
