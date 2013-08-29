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


QQuaternion Quaternion::fromTo( const QVector3D & fromDirection, const QVector3D & toDirection )
{
	QVector3D axis = QVector3D::crossProduct( fromDirection, toDirection );
	float angle = acosf( QVector3D::dotProduct( fromDirection, toDirection ) ) * (180.0/M_PI);
	return QQuaternion::fromAxisAndAngle( axis, angle );
}
