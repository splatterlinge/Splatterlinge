#include "quaternion.hpp"
#include "vector.hpp"

#include <math.h>
#include <float.h>


QQuaternion quaternionLookAt( const QVector3D & lookAt, const QVector3D & upDirection )
{
	//BUG: this is not very stable
	QVector3D forward = lookAt.normalized();
	QVector3D up = vectorOrthoNormalize( lookAt, upDirection );
	if( up.isNull() )
		return QQuaternion();
	QVector3D right = QVector3D::crossProduct( up, forward );

	double ruf = 1.0 + right.x() + up.y() + forward.z();
	if( ruf < FLT_EPSILON )
		return QQuaternion();
	double scalar = sqrt( ruf ) * 0.5f;
	if( scalar < FLT_EPSILON )
		return QQuaternion();
	double w4Recip = 1.0f / ( 4.0f * scalar );
	return QQuaternion
	(
		scalar,
		( up.z() - forward.y() ) * w4Recip,
		( forward.x() - right.z() ) * w4Recip,
		( right.y() - up.x() ) * w4Recip
	);
}
