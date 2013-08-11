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


Forest::Forest( World * world, QString filename, QPointF position, int radius, int number ) :
	AWorldObject( world ),
	mTerrain( world->landscape()->terrain() )
{
	mModel = new StaticModel( world->scene(), filename );

	for( int i=0; i<number; i++ )
	{
		QMatrix4x4 pos;
		QVector2D random = RandomNumber::inUnitCircle();

		float x = position.x() + random.x() * radius;
		float y = position.y() + random.y() * radius;
		float z = mTerrain->getHeight( QPointF(x,y) );

		if( z >= -9 )
		{
			pos.translate( x, z-1, y );
			pos.scale( 0.3f+(float)rand()/( (float)RAND_MAX/0.3 ) );
			pos.rotate( qrand() % 360, 0.0, 1.0, 0.0 );
			pos.rotate( qrand() % 5, 1.0, 0.0, 1.0 );

			mInstances.append(pos);
		}
	}

	setBoundingSphere( radius );
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
	mModel->draw( mInstances );
}
