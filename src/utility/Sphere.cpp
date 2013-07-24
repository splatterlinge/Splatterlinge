#include "Sphere.hpp"

#include <math.h>


bool Sphere::intersectSphere( const QVector3D & centerA, const float & radiusA, const QVector3D & centerB, const float & radiusB, float * depth )
{
	QVector3D relPos = centerB - centerA;

	float squaredDistance = relPos.lengthSquared();

	float squaredRadi = radiusA + radiusB;
	squaredRadi *= squaredRadi;

	if( squaredRadi < squaredDistance )
		return false;

	if( !depth )
		return true;

	*depth = sqrtf( squaredRadi ) - sqrtf( squaredDistance );
	return true;
}


// http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection

bool Sphere::intersectRay( const QVector3D & sphereCenter, const float & sphereRadius, const QVector3D & rayOrigin, const QVector3D & rayDirection, float * intersectionDistance )
{
	QVector3D relRayOrigin = rayOrigin - sphereCenter;

	// compute coefficients
	float a = rayDirection.lengthSquared();
	float b = 2.0f * QVector3D::dotProduct( rayDirection, relRayOrigin );
	float c = relRayOrigin.lengthSquared() - (sphereRadius * sphereRadius);

	// find discriminant
	float disc = b * b - 4 * a * c;

	// if discriminant is negative there are no real roots, so return false as ray misses sphere
	if( disc < 0.0f )
		return false;

	float distSqrt = sqrtf( disc );
	float q;
	if( b < 0.0f )
		q = (-b - distSqrt)/2.0f;
	else
		q = (-b + distSqrt)/2.0f;

	// compute t0 and t1
	float t0 = q / a;
	float t1 = c / q;

	// make sure t0 is smaller than t1
	if( t0 > t1 )
	{	// if t0 is bigger than t1 swap them around
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	// if t1 is less than zero, the object is in the ray's negative direction
	// and consequently the ray misses the sphere
	if( t1 < 0.0f )
		return false;

	// skip the rest if not interested in distance
	if( !intersectionDistance )
		return true;

	// if t0 is less than zero, the intersection point is at t1
	if( t0 < 0.0f )
		*intersectionDistance = t1;
	else	// else the intersection point is at t0
		*intersectionDistance = t0;
	return true;
}
