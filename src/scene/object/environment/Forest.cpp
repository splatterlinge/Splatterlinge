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

#include "Forest.hpp"
#include <scene/object/World.hpp>


Forest::Forest( World * world, Terrain * terrain, const QString & filename, const QPoint & mapPosition, int mapRadius, int number ) :
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
			pos.scale( RandomNumber::minMax( 0.3, 0.4 ) );
			pos.rotate( qrand() % 360, 0.0, 1.0, 0.0 );
			pos.rotate( qrand() % 5, 1.0, 0.0, 1.0 );

			mInstances.append(pos);
		}
	}

	setPosition( QVector3D( position.x(), 0, position.y() ) );
	setBoundingSphere( qMax(radi.width(),radi.height()) + qMax(mModel->size().width(),mModel->size().height()) * 0.4f * 2.0f );
}


Forest::~Forest()
{
	delete mModel;
}


void Forest::updateSelf( const double & delta )
{
}


void Forest::drawSelf()
{
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_CULL_FACE );
	mModel->draw( mInstances );
	glPopAttrib();
}
