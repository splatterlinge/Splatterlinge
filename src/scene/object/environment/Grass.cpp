#include "Grass.hpp"
#include <scene/object/World.hpp>
#include <utility/Capsule.hpp>
#include <utility/Sphere.hpp>

Grass::Grass( World * world, Terrain * terrain, const QString & filename, const QPoint & mapPosition, int mapRadius, int number ) :
	AWorldObject( world ),
	mTerrain( terrain )
{
	QPointF position = mTerrain->fromMap( mapPosition );
	QSizeF radi = mTerrain->fromMap( QSize( mapRadius, mapRadius ) );

	mModel = new StaticModel( world->scene(), filename );
	for( int i=0; i<number; i++ )
	{
		QMatrix4x4 pos;
		QVector2D random = RandomNumber::inUnitCircle();

		float x = position.x() + random.x() * radi.width();
		float z = position.y() + random.y() * radi.height();
		float y = mTerrain->getHeight( QPointF(x,z) ) - 1.0f;

		if( y >= -9 )
		{
			pos.translate( x, y, z );
			pos.scale( RandomNumber::minMax( 0.5f, 0.9f ) );
			pos.rotate( QQuaternion::nlerp( rotation(), terrain->getNormalRotation( mapPosition ), -90.0 ) );

			mInstances.append(pos);
		}
	}

	setPosition( QVector3D( position.x(), 0, position.y() ) );
	setBoundingSphere( qMax(radi.width(),radi.height()) + qMax(mModel->size().width(),mModel->size().height()) * 0.8f * 1.5f );
}

Grass::~Grass()
{
}

void Grass::updateSelf( const double & delta )
{
}


void Grass::drawSelf()
{
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_CULL_FACE );
	mModel->draw( mInstances );
	glPopAttrib();
}
