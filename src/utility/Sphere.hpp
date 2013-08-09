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

#ifndef UTILITY_SPHERE_INCLUDED
#define UTILITY_SPHERE_INCLUDED


#include <QVector3D>


/// Sphere
class Sphere
{
public:
	Sphere() : mCenter(), mRadius(0.0f) {}
	Sphere( const QVector3D & center, const float & radius ) : mCenter(center), mRadius(radius) {}
	const QVector3D & center() const { return mCenter; }
	const float & radius() const { return mRadius; }
	void setCenter( const QVector3D & center ) { mCenter = center; }
	void setRadius( const float & radius ) { mRadius = radius; }

	bool intersectSphere( const QVector3D & centerB, const float & radiusB, QVector3D * normal, float * depth ) const
		{ return intersectSphere( mCenter, mRadius, centerB, radiusB, normal, depth ); }

	bool intersectRay( const QVector3D & origin, const QVector3D & direction, float * intersectionDistance ) const
		{ return intersectRay( mCenter, mRadius, origin, direction, intersectionDistance ); }

	static bool intersectSphere( const QVector3D & centerA, const float & radiusA,
		const QVector3D & centerB, const float & radiusB,
		QVector3D * normal, float * depth );

	static bool intersectRay( const QVector3D & sphereCenter, const float & sphereRadius,
		const QVector3D & rayOrigin, const QVector3D & rayDirection,
		float * intersectionDistance );

private:
	QVector3D mCenter;
	float mRadius;
};


#endif
