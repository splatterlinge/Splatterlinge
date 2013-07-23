#include "vector.hpp"


QVector3D vectorOrthoNormalize( const QVector3D & fixed, const QVector3D & v )
{
	QVector3D proj = fixed * QVector3D::dotProduct( v, fixed );
	return (v - proj).normalized();
}
