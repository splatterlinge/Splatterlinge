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

#include "glWrappers.hpp"


QString glGetErrorString( const GLenum & error )
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


GLenum glGetAlphaTestFunctionFromString( const QString & name )
{
	if( name == "GL_NEVER" ) return GL_NEVER;
	else if( name == "GL_LESS" ) return GL_LESS;
	else if( name == "GL_EQUAL" ) return GL_EQUAL;
	else if( name == "GL_LEQUAL" ) return GL_LEQUAL;
	else if( name == "GL_GREATER" ) return GL_GREATER;
	else if( name == "GL_NOTEQUAL" ) return GL_NOTEQUAL;
	else if( name == "GL_GEQUAL" ) return GL_GEQUAL;
	else if( name == "GL_ALWAYS" ) return GL_ALWAYS;
	else return GL_ALWAYS;
}


GLint glGetTextureWrapFromString( const QString & name )
{
	if( name == "GL_CLAMP_TO_EDGE" ) return GL_CLAMP_TO_EDGE;
	else if( name == "GL_CLAMP_TO_BORDER" ) return GL_CLAMP_TO_BORDER;
	else if( name == "GL_MIRRORED_REPEAT" ) return GL_MIRRORED_REPEAT;
	else return GL_REPEAT;
}
