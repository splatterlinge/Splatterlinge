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

	bool intersectRay( const QVector3D & origin, const QVector3D & direction, float * intersectionDistance ) const
		{ return intersectRay( mCenter, mRadius, origin, direction, intersectionDistance ); }

	static bool intersectRay( const QVector3D & sphereCenter, const float & sphereRadius, const QVector3D & rayOrigin, const QVector3D & rayDirection, float * intersectionDistance );

private:
	QVector3D mCenter;
	float mRadius;
};


#endif
