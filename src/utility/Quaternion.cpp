#include "Quaternion.hpp"
#include "Vector.hpp"

#include <math.h>
#include <float.h>


// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

QQuaternion Quaternion::lookAt( const QVector3D & lookAt, const QVector3D & upDirection )
{
	QQuaternion ret;
	QVector3D forward = lookAt.normalized();
	QVector3D up = Vector::orthoNormalize( lookAt, upDirection );
	if( up.isNull() )	// we are looking parallel to the up vector
		return QQuaternion();
	QVector3D right = QVector3D::crossProduct( up, forward );

	float trace = right.x() + up.y() + forward.z();
	if( trace > 0.0 )
	{
		float s = 0.5f / sqrtf( trace + 1.0f );
		ret.setScalar( 0.25f / s );
		ret.setX( ( up.z() - forward.y() ) * s );
		ret.setY( ( forward.x() - right.z() ) * s );
		ret.setZ( ( right.y() - up.x() ) * s );
	} else {
		if( right.x() > up.y() && right.x() > forward.z() ) {
			float s = 2.0f * sqrtf( 1.0f + right.x() - up.y() - forward.z() );
			ret.setScalar( ( up.z() - forward.y() ) / s );
			ret.setX( 0.25f * s );
			ret.setY( ( up.x() + right.y() ) / s );
			ret.setZ( ( forward.x() + right.z() ) / s );
		} else if( up.y() > forward.z() ) {
			float s = 2.0f * sqrtf( 1.0f + up.y() - right.x() - forward.z() );
			ret.setScalar( ( forward.x() - right.z() ) / s );
			ret.setX( ( up.x() + right.y() ) / s );
			ret.setY( 0.25f * s );
			ret.setZ( ( forward.y() + up.z() ) / s );
		} else {
			float s = 2.0f * sqrtf( 1.0f + forward.z() - right.x() - up.y() );
			ret.setScalar( ( right.y() - up.x() ) / s );
			ret.setX( ( forward.x() + right.z() ) / s );
			ret.setY( ( forward.y() + up.z() ) / s );
			ret.setZ( 0.25f * s );
		}
	}
	return ret;
}
