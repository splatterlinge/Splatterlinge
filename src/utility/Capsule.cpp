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

#include "Capsule.hpp"

#include <math.h>
#include <float.h>


// http://stackoverflow.com/questions/15310239/collision-detection-response-between-a-moving-sphere-and-a-circular-cylinder

bool Capsule::intersectSphere( const QVector3D & startCapsule, const QVector3D & endCapsule, const float & radiusCapsule,
	const QVector3D & centerSphere, const float & radiusSphere, QVector3D * normal, float * depth )
{
	QVector3D cylCenterVector = endCapsule - startCapsule;
	float distanceFactorFromStart = QVector3D::dotProduct( centerSphere - startCapsule, cylCenterVector  ) / cylCenterVector.lengthSquared();
	if( distanceFactorFromStart < 0.0f ) distanceFactorFromStart = 0.0f;	// clamp to endpoints if necessary
	if( distanceFactorFromStart > 1.0f ) distanceFactorFromStart = 1.0f;
	QVector3D closestPoint = startCapsule + ( cylCenterVector * distanceFactorFromStart );

	QVector3D collisionVector = centerSphere - closestPoint;
	float distance = collisionVector.length();
	float radi = radiusSphere + radiusCapsule;

	if( distance < radi )
	{
		if( normal && distance > FLT_EPSILON )
			*normal = collisionVector / distance;
		if( depth )
			*depth = -(distance - radi);
		return true;
	}
	return false;
}
