#include "Triangle.hpp"

#include "float.h"


bool Triangle::intersectRay( const QVector3D & origin, const QVector3D & direction, float * intersectionDistance )
{
	QVector3D edge1 = mQ - mP;
	QVector3D edge2 = mR - mP;

	QVector3D pVec = QVector3D::crossProduct( direction, edge2 );

	float det = QVector3D::dotProduct( edge1, pVec );
	if( det < FLT_EPSILON )
		return false;

	QVector3D tVec = origin - mP;
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
