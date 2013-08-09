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

#ifndef UTILITY_CAPSULE_INCLUDED
#define UTILITY_CAPSULE_INCLUDED


#include <QVector3D>


/// Capsule
class Capsule
{
public:
	Capsule() : mStart(), mEnd(), mRadius(0.0f) {}
	Capsule( const QVector3D & start, const QVector3D & end, const float & radius ) : mStart(start), mEnd(end), mRadius(radius) {}
	const QVector3D & start() const { return mStart; }
	const QVector3D & end() const { return mEnd; }
	const float & radius() const { return mRadius; }
	void setStart( const QVector3D & start ) { mStart = start; }
	void setEnd( const QVector3D & end ) { mEnd = end; }
	void setRadius( const float & radius ) { mRadius = radius; }
/*
	bool intersectRay( const QVector3D & origin, const QVector3D & direction, float * intersectionDistance ) const
		{ return intersectRay( mStart, mEnd, mRadius, origin, direction, intersectionDistance ); }
*/
	static bool intersectSphere( const QVector3D & startCapsule, const QVector3D & endCapsule, const float & radiusCapsule,
		const QVector3D & centerSphere, const float & radiusSphere, QVector3D * normal, float * depth );
//	static bool intersectRay( const QVector3D & sphereCenter, const float & sphereRadius, const QVector3D & rayOrigin, const QVector3D & rayDirection, float * intersectionDistance );

private:
	QVector3D mStart;
	QVector3D mEnd;
	float mRadius;
};


#endif
