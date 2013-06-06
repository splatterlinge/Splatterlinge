#include "glWrappers.hpp"


QString glGetErrorString( GLenum error )
{
	switch( error )
	{
		case GL_NO_ERROR:
			return QString( "No Error" );
		case GL_INVALID_ENUM:
			return QString( "Invalid Enumeration" );
		case GL_INVALID_VALUE:
			return QString( "Invalid Value" );
		case GL_INVALID_OPERATION:
			return QString( "Invalid Operation" );
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return QString( "Invalid Framebuffer Operation" );
		case GL_OUT_OF_MEMORY:
			return QString( "Out Of Memory" );
		case GL_STACK_OVERFLOW:
			return QString( "Stack Overflow" );
		case GL_STACK_UNDERFLOW:
			return QString( "Stack Underflow" );
		case GL_TABLE_TOO_LARGE:
			return QString( "Table Too Large" );
	}
	return QString( "Unknown Error" );
}
