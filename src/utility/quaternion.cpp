#include "quaternion.hpp"
#include "vector.hpp"

#include <math.h>


QQuaternion quaternionLookAt( const QVector3D & lookAt, const QVector3D & upDirection )
{
	QVector3D forward = lookAt.normalized();
	QVector3D up = vectorOrthoNormalize( lookAt, upDirection );
	QVector3D right = QVector3D::crossProduct( up, forward );

	float scalar = sqrtf( 1.0f + right.x() + up.y() + forward.z() ) * 0.5f;
	float w4Recip = 1.0f / ( 4.0f * scalar );
	return QQuaternion
	(
		scalar,
		( up.z() - forward.y() ) * w4Recip,
		( forward.x() - right.z() ) * w4Recip,
		( right.y() - up.x() ) * w4Recip
	);
}
