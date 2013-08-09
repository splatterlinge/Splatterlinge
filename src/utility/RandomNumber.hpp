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

#ifndef UTILITY_RANDOMNUMBER_INCLUDED
#define UTILITY_RANDOMNUMBER_INCLUDED

#include <stdlib.h>
#include <QVector3D>
#include <QVector2D>
#include <QtGlobal>


namespace RandomNumber
{
	inline float minMax( const float & min, const float & max)
		{ return ( (float)qrand()/(float)RAND_MAX ) * (max-min+1) + min; }

	QVector3D inUnitSphere();
	QVector2D inUnitCircle();
}


#endif
