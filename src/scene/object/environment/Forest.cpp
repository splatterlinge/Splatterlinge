#include "Forest.hpp"

Forest::Forest( Scene * scene, Terrain * terrain, QString filename, QPointF position, int radius, int number ) :
	WavefrontObject( scene, filename ),
	mTerrain( terrain )
{
	for( int i=0; i<number; i++ )
	{
		QMatrix4x4 pos;
		float t = 2 * M_PI * qrand();
		float u = qrand() % radius/2 + qrand() % radius/2;
		float r = u>1 ? 2-u : u;

		float x = position.x() + r*cos(t);
		float y = position.y() + r*sin(t);
		float z = mTerrain->getHeight( QPointF(x,y) );

		if( z >= -9 )
		{
			pos.translate( x, z-1, y );
			pos.scale( 0.3f+(float)rand()/( (float)RAND_MAX/0.3 ) );
			pos.rotate( qrand() % 360, 0.0, 1.0, 0.0 );
			pos.rotate( qrand() % 5, 1.0, 0.0, 1.0 );

			mInstances->append(pos);
		}
		else
		{
			qDebug() << x << y;
		}
	}

	setBoundingSphere( radius );
}

Forest::~Forest()
{
}
