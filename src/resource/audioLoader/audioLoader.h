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
