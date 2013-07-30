#include "RandomNumber.hpp"


QVector3D RandomNumber::inUnitSphere()
{
	QVector3D vec;
	do {
		vec = QVector3D( RandomNumber::minMax( -1.0, 1.0 ), RandomNumber::minMax( -1.0, 1.0 ), RandomNumber::minMax( -1.0, 1.0 ) );
	} while( vec.lengthSquared() > 1.0f );
	return vec;
}
