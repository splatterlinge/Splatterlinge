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
#include <utility/Capsule.hpp>
#include <utility/Sphere.hpp>


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
			pos.scale( RandomNumber::minMax( 0.1f, 0.25f ) );
			pos.rotate( RandomNumber::minMax( -10.0f, 10.0f ), 1.0f, 0.0f, 0.0f );
			pos.rotate( RandomNumber::minMax( 0.0f, 360.0f ), 0.0f, 1.0f, 0.0f );
			pos.rotate( RandomNumber::minMax( -10.0f, 10.0f ), 1.0f, 0.0f, 0.0f );

			mInstances.append(pos);
		}
	}

	setPosition( QVector3D( position.x(), 0, position.y() ) );
	setBoundingSphere( qMax(radi.width(),radi.height()) + qMax(mModel->size().width(),mModel->size().height()) * 0.8f * 1.5f );
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


QVector<AObject*> Forest::collideSphere( const AObject * exclude, const float & radius, QVector3D & center, QVector3D * normal )
{
	QVector<AObject*> collides = AObject::collideSphere( exclude, radius, center, normal );
	float depth;
	QVector3D tmpNormal;

	if( !Sphere::intersectSphere( position(), boundingSphereRadius(), center, radius, &tmpNormal, &depth ) )
		return collides;	// return if we aren't even near the forest

	for( QVector<QMatrix4x4>::iterator i = mInstances.begin(); i != mInstances.end(); ++i )
	{
		float treeScale = (*i).column(0).length();
		QVector3D treeBottom = (*i).column(3).toVector3D();
		QVector3D treeTop = (*i).column(3).toVector3D() + (*i).mapVector( QVector3D(0,60,0) );
		if( Capsule::intersectSphere( treeBottom, treeTop, treeScale, center, radius, &tmpNormal, &depth ) )
		{
			collides.append( this );
			center += tmpNormal * depth;
			if( normal )
				*normal = tmpNormal;
		}
	}

	return collides;
}
