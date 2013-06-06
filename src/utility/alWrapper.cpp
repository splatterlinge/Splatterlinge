#include "alWrappers.hpp"


QString alGetErrorString( ALenum error )
{
	switch( error )
	{
		case AL_NO_ERROR:
			return QString( "No Error" );
		case AL_INVALID_NAME:
			return QString( "Invalid Enumeration" );
		case AL_INVALID_ENUM:
			return QString( "Invalid Enumeration" );
		case AL_INVALID_VALUE:
			return QString( "Invalid Value" );
		case AL_INVALID_OPERATION:
			return QString( "Invalid Operation" );
		case AL_OUT_OF_MEMORY:
			return QString( "Out Of Memory" );
	}
	return QString( "Unknown Error" );
}
