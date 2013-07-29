#ifndef UTILITY_QUATERNION_INCLUDED
#define UTILITY_QUATERNION_INCLUDED


#include <QQuaternion>


namespace Quaternion
{
	QQuaternion lookAt( const QVector3D & lookAt, const QVector3D & upDirection );
}


#endif
