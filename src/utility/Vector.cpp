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

#include "Vector.hpp"

#include <math.h>
#include <float.h>

#include <QDebug>


QVector3D Vector::orthoNormalize( const QVector3D & fixed, const QVector3D & v )
{
	QVector3D proj = fixed * QVector3D::dotProduct( v, fixed );
	QVector3D ortho = v - proj;
	float length = ortho.length();
	if( length <= FLT_EPSILON )
		return QVector3D();
	return ortho/length;
}
