/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
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

#include "Dummy.hpp"

#include <scene/object/World.hpp>
#include <resource/Material.hpp>
#include <effect/SplatterSystem.hpp>
#include <utility/RandomNumber.hpp>
#include <utility/Quaternion.hpp>
#include <utility/Sphere.hpp>

#include <math.h>
#include <float.h>


Dummy::Dummy( World * world ) : ACreature( world )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mHeightAboveGround = 6.0f;
	mVelocityY = 0.0f;
	mMaterial = new Material( scene()->glWidget(), "KirksEntry" );
}


Dummy::~Dummy()
{
	gluDeleteQuadric( mQuadric );
	delete mMaterial;
}


static QVector3D randomPointOnWorld( World * world )
{
	QVector3D pos( RandomNumber::minMax(-100,100), 0, RandomNumber::minMax(-100,100) );
	pos.setY( world->landscape()->terrain()->getHeight( pos ) );
	return pos;
}


void Dummy::updateSelf( const double & delta )
{
	switch( state() )
	{
		case SPAWNING:
		{
			setPosition( randomPointOnWorld( world() ) + QVector3D(0,10,0) );
			setState( ALIVE );
			setLife( 100 );
			mHeightAboveGround = 6.0f;
			break;
		}

		case ALIVE:
		{
			mTarget = world()->teapot()->worldPosition();
			QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
			QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D(0,1,0) );
			setRotation( QQuaternion::slerp( rotation(), targetRotation, 0.05 ) );
			setPosition( position() + direction()*delta*10.0 );
			if( life() <= 0 )
				setState( DYING );
			break;
		}

		case DYING:
			setState( DEAD );
			mHeightAboveGround = 3.0f;
			break;

		case DEAD:
			break;
	}

	mVelocityY += -3.0f * delta;	// apply some gravity
	setPositionY( position().y() + mVelocityY * delta );

	float landscapeHeight;
	if( world()->landscape()->terrain()->getHeight( position(), landscapeHeight ) )
	{
		if( landscapeHeight + mHeightAboveGround > position().y() )
		{
			setPositionY( landscapeHeight + mHeightAboveGround );
			if( mVelocityY < 0.0f )
				mVelocityY = 0.0f;
		}
	}
}


void Dummy::drawSelf()
{
	mMaterial->bind();
	gluSphere( mQuadric, 4, 8, 8 );
	mMaterial->release();

	glDisable( GL_LIGHTING );
	glBegin( GL_LINES );
		glColor(0.0,0.0,1.0);
		glVertex(0,0,0);
		glVertex(0,0,10);

		glColor(0.0,1.0,0.0);
		glVertex(0,0,0);
		glVertex(0,10,0);

		glColor(1.0,0.0,0.0);
		glVertex(0,0,0);
		glVertex(10,0,0);
	glEnd();
}


AObject * Dummy::intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction, float & length, QVector3D * normal )
{
	AObject * nearestTarget = AObject::intersectLine( exclude, origin, direction, length, normal );

	float rayLength;
	if( Sphere::intersectCulledRay( worldPosition(), 4, origin, direction, &rayLength ) )
	{
		if( rayLength < length )
		{	// intersection closer than previous intersections?
			length = rayLength;
			if( normal )	// interested in normal?
				*normal = origin - worldPosition();
			nearestTarget = this;
		}
	}

	return nearestTarget;
}


void Dummy::receiveDamage( int damage, const QVector3D * position, const QVector3D * direction )
{
	ACreature::receiveDamage( damage, direction );
	QVector3D splatterSource;
	if( position )
		splatterSource = *position;
	else
		splatterSource = worldPosition();

	if( state() != DEAD )
		world()->splatterSystem()->spray( splatterSource, damage );
	else
		world()->splatterSystem()->spray( splatterSource, damage/2.0f );
}
