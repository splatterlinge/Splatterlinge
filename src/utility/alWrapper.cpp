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
