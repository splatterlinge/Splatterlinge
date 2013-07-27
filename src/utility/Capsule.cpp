#include "Capsule.hpp"

#include <math.h>


// http://stackoverflow.com/questions/15310239/collision-detection-response-between-a-moving-sphere-and-a-circular-cylinder

bool Capsule::intersectSphere( const QVector3D & startCapsule, const QVector3D & endCapsule, const float & radiusCapsule,
	const QVector3D & centerSphere, const float & radiusSphere, QVector3D * normal, float * depth )
{
	QVector3D cylCenterVector = endCapsule - startCapsule;
	float distanceFactorFromStart = QVector3D::dotProduct( centerSphere - startCapsule, cylCenterVector  ) / cylCenterVector.lengthSquared();
	if( distanceFactorFromStart < 0.0f ) distanceFactorFromStart = 0.0f;	// clamp to endpoints if neccesary
	if( distanceFactorFromStart > 1.0f ) distanceFactorFromStart = 1.0f;
	QVector3D closestPoint = startCapsule + ( cylCenterVector * distanceFactorFromStart );

	QVector3D collisionVector = centerSphere - closestPoint;
	float distance = collisionVector.length();
	float radi = radiusSphere + radiusCapsule;

	if( distance < radi )
	{
		if( normal )
			*normal = collisionVector / distance;
		if( depth )
			*depth = -(distance - radi);
		return true;
	}
	return false;
}
