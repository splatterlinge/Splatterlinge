#include "Grass.hpp"

#include <scene/object/World.hpp>
#include <scene/object/Landscape.hpp>
#include <utility/Capsule.hpp>
#include <utility/Sphere.hpp>


Grass::Grass( Landscape * landscape, const QString & filename, const QPoint & mapPosition, int mapRadius, int number ) :
	AWorldObject( landscape->world() ),
	mLandscape( landscape )
{
	QPointF position = mLandscape->terrain()->fromMap( mapPosition );
	QSizeF radi = mLandscape->terrain()->fromMap( QSize( mapRadius, mapRadius ) );

	mModel = new StaticModel( world()->scene(), filename );
	setPosition( QVector3D( position.x(), 0, position.y() ) );
	setBoundingSphere( qMax(radi.width(),radi.height()) + qMax(mModel->size().width(),mModel->size().height()) * 0.8f * 1.5f );

	for( int i=0; i<number; i++ )
	{
		QVector3D treePos;
		int tries = 0;
		do {
			QVector2D random = RandomNumber::inUnitCircle();
			treePos.setX( position.x() + random.x() * radi.width() );
			treePos.setZ( position.y() + random.y() * radi.height() );
			treePos.setY( mLandscape->terrain()->getHeight( QPointF( treePos.x(),treePos.z()) ) - 1.0f );
			if( tries > 1000 )
			{
				qWarning() << QObject::tr("Giving up placing grass - no suitable position found");
				return;
			}
		} while( treePos.y() < mLandscape->waterHeight() );

		QMatrix4x4 pos;
		pos.translate( treePos );
		pos.scale( RandomNumber::minMax( 0.4f, 0.7f ) );
		pos.rotate( mLandscape->terrain()->getNormalRotation( treePos ) );

		mInstances.append(pos);
	}
}


Grass::~Grass()
{
}


void Grass::updateSelf( const double & delta )
{
}


void Grass::drawSelf()
{
	mModel->draw( mInstances );
}
