#include "Vector.hpp"

#include <math.h>
#include <float.h>

#include <QDebug>


QVector3D Vector::orthoNormalize( const QVector3D & fixed, const QVector3D & v )
{
	QVector3D proj = fixed * QVector3D::dotProduct( v, fixed );
	QVector3D ortho = v - proj;
	float length = ortho.length();
	if( length <= FLT_EPSILON )
		return QVector3D();
	return ortho/length;
}
