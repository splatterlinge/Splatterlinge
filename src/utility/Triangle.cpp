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

#include "Triangle.hpp"

#include "float.h"


bool Triangle::intersectRay( const QVector3D & p, const QVector3D & q, const QVector3D & r, const QVector3D & origin, const QVector3D & direction, float * intersectionDistance )
{
	QVector3D edge1 = q - p;
	QVector3D edge2 = r - p;

	QVector3D pVec = QVector3D::crossProduct( direction, edge2 );

	float det = QVector3D::dotProduct( edge1, pVec );
	if( det < FLT_EPSILON )
		return false;

	QVector3D tVec = origin - p;
	float u = QVector3D::dotProduct( tVec, pVec );
	if( u < 0.0f || u > det )
		return false;

	QVector3D qVec = QVector3D::crossProduct( tVec, edge1 );
	float v = QVector3D::dotProduct( direction, qVec );
	if( v < 0.0f || u+v > det )
		return false;

	if( intersectionDistance )
		*intersectionDistance = QVector3D::dotProduct( edge2, qVec ) / det;

	return true;
}


bool Triangle::intersectCulledRay( const QVector3D & p, const QVector3D & q, const QVector3D & r, const QVector3D & origin, const QVector3D & direction, float * intersectionDistance )
{
	float distance;
	bool intersection = Triangle::intersectRay( p, q, r, origin, direction, &distance );
	if( intersection && distance > 0.0f )
	{
		if( intersectionDistance )
			*intersectionDistance = distance;
		return true;
	}
	return false;
}
