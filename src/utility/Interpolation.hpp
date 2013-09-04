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

#ifndef UTILITY_INTERPOLATION_INCLUDED
#define UTILITY_INTERPOLATION_INCLUDED

#include <math.h>


namespace Interpolation
{
	///Linear interpolation between a and b.
	template< class T > inline T linear( const T & a, const T & b, const double & w )
		{ return a*(1.0-w)+b*w; }

	///Linear interpolation between a and b.
	template< class T > inline T linear( const T & a, const T & b, const float & w )
		{ return a*(1.0f-w)+b*w; }

	///Cosine interpolation between a and b.
	template< class T > inline T cosine( const T & a, const T & b, const double & w )
		{ double f=(1.0-cos(w*M_PI))*0.5; return a*(1.0-f)+b*f; }

	///Cosine interpolation between a and b.
	template< class T > inline T cosine( const T & a, const T & b, const float & w )
		{ float f=(1.0f-cosf(w*(float)M_PI))*0.5f; return a*(1.0f-f)+b*f; }
}


#endif
