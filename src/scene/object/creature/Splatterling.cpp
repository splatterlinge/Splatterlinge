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

#include "Splatterling.hpp"

#include <scene/object/World.hpp>
#include <resource/Material.hpp>
#include <effect/SplatterSystem.hpp>
#include <utility/RandomNumber.hpp>
#include <utility/Intersection.hpp>
#include <utility/Quaternion.hpp>
#include <utility/Sphere.hpp>

#include <math.h>
#include <float.h>
#include <QDebug>


static const GLfloat GlobalPositionData[] =
{
	0.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 8.0f,
	1.0f, 1.0f, 8.0f,
	1.0f, -1.0f, 8.0f,
	-1.0f, -1.0f, 8.0f,
	-1.0f, 1.0f, 8.0f,

	0.0f,0.0f,0.0f,
	0.25f,0.5f,1.0f,
	-0.25f,0.5f,1.0f,
	-0.5f,0.25f,1.0f,
	-0.5f,-0.25f,1.0f,
	-0.25f,-0.5f,1.0f,
	0.25f,-0.5f,1.0f,
	0.5f,-0.25f,1.0f,
	0.5f,0.25f,1.0f,
	0.25f,0.5f,1.0f,

	0.25f,0.5f,1.0f,
	0.5f,1.0f,8.0f,
	-0.25f,0.5f,1.0f,
	-0.5f,1.0f,8.0f,
	-0.5f,0.25f,1.0f,
	-1.0f,0.5f,8.0f,
	-0.5f,-0.25f,1.0f,
	-1.0f,-0.5f,8.0f,
	-0.25f,-0.5f,1.0f,
	-0.5f,-1.0f,8.0f,
	0.25f,-0.5f,1.0f,
	0.5f,-1.0f,8.0f,
	0.5f,-0.25f,1.0f,
	1.0f,-0.5f,8.0f,
	0.5f,0.25f,1.0f,
	1.0f,0.5f,8.0f,
	0.25f,0.5f,1.0f,
	0.5f,1.0f,8.0f,

	//face

	0.0f,0.0f,8.8f,
	0.25f,0.5f,8.5f,
	-0.25f,0.5f,8.5f,
	-0.5f,0.25f,8.5f,
	-0.5f,-0.25f,8.5f,
	-0.25f,-0.5f,8.5f,
	0.25f,-0.5f,8.5f,
	0.5f,-0.25f,8.5f,
	0.5f,0.25f,8.5f,
	0.25f,0.5f,8.5f,

	0.25f,0.5f,8.5f,
	0.5f,1.0f,8.0f,
	-0.25f,0.5f,8.5f,
	-0.5f,1.0f,8.0f,
	-0.5f,0.25f,8.5f,
	-1.0f,0.5f,8.0f,
	-0.5f,-0.25f,8.5f,
	-1.0f,-0.5f,8.0f,
	-0.25f,-0.5f,8.5f,
	-0.5f,-1.0f,8.0f,
	0.25f,-0.5f,8.5f,
	0.5f,-1.0f,8.0f,
	0.5f,-0.25f,8.5f,
	1.0f,-0.5f,8.0f,
	0.5f,0.25f,8.5f,
	1.0f,0.5f,8.0f,
	0.25f,0.5f,8.5f,
	0.5f,1.0f,8.0f,

	-1.0f, 1.0f, 8.0f,
	-1.0f, -1.0f, 8.0f,
	1.0f, 1.0f, 8.0f,
	1.0f, -1.0f, 8.0f,

	//wingOne
	0.0f, 0.5f, 4.0f,
	8.0f, 4.0f, -6.0f,
	8.0f, 4.0f, 14.0f,

	//wingTwo
	0.0f, 0.5f, 4.0f,
	-8.0f, 4.0f, -6.0f,
	-8.0f, 4.0f, 14.0f,
};

static GLfloat TextureCoordData[] =
{
	0.0f, 0.0f,
	0.5f, 0.0f,
	0.5f, 0.5f,
	0.5f, 0.0f,
	0.5f, 0.5f,
	0.5f, 0.0f,


	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,


	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,

	//face

	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,

	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,
	0.7f, 0.65f,

	0.0f, 1.0f,
	0.0f, 0.5f,
	0.5f, 1.0f,
	0.5f, 0.5f,

	//wingOne
	0.5f, 0.65f,
	1.0f, 0.25f,
	1.0f, 1.0f,

	//wingTwo
	0.5f, 0.65f,
	1.0f, 0.25f,
	1.0f, 1.0f,

};


static const GLubyte ColorData[] =
{
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,

	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,

	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,

	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,
	255, 255, 255,

	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,
	0, 255, 0,

	0, 255, 255,
	0, 255, 255,
	0, 255, 255,
	0, 255, 255,

	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
};

static const GLfloat GlobalNormalData[] =
{
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,

	0.0f, 0.0f, -1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, -1.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f,

	//face
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	//wingOne
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	//wingTwo
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};

static void initTexCoordArray()
{
	int posCoordIndex, end = Splatterling::TexSize / sizeof( GLfloat );
	end = end - (10 * 2);

	for( int i = Splatterling::BodyVertexCount * 2; i < end; i = i + 2 )
	{
		posCoordIndex = ( i / 2 ) * 3;
		TextureCoordData[i] = ( (GlobalPositionData[posCoordIndex] + 1.0f) / 2.0f) / 2.0f;
		TextureCoordData[i + 1] = ( (GlobalPositionData[posCoordIndex + 1] + 1.0f) / 2.0f) / 2.0f + 0.5f;
	}
}


Splatterling::Splatterling( World * world ) : ACreature( world )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mHeightAboveGround = 1.0f;
	mVelocityY = 0.0f;
	mMaterial = new Material( scene()->glWidget(), "Splatterling" );
	glGenBuffers( BufferSize, this->BufferName );

	wingUpMovement = false;
	playerDetected = false;
	mWingRightDisintegrated = false;
	mWingLeftDisintegrated = false;
	mHeadDisintegrated = false;
	mBodyHittedToGround = false;

	mWingSound = new AudioSample( "butterfly" );
	mWingSound->setLooping( true );
	mWingSound->setRolloffFactor( 0.01f );
	mWingSound->play();

	mSnapSound = new AudioSample( "neck_snap" );
	mSnapSound->setLooping( false );
	mSnapSound->setRolloffFactor( 0.01f );

	damageMultiplicationFactor[TARGET_BODY] = 2.0f;
	damageMultiplicationFactor[TARGET_HEAD] = 3.0f;
	damageMultiplicationFactor[TARGET_WING_LEFT] = 1.0f;
	damageMultiplicationFactor[TARGET_WING_RIGHT] = 1.0f;

	mDamageOnBodyPart[TARGET_BODY] = 0.0f;
	mDamageOnBodyPart[TARGET_HEAD] = 0.0f;
	mDamageOnBodyPart[TARGET_WING_LEFT] = 0.0f;
	mDamageOnBodyPart[TARGET_WING_RIGHT] = 0.0f;

	for( unsigned int i = 0; i < PositionSize / sizeof( GLfloat ); i++ )
	{
		PositionData[i] = GlobalPositionData[i] * Splatterling::SplatterlingSizeFactor;
	}

	mCoolDown = 0.0f;
	recalculationOfRotationAngle = true;
	rotationAroundPlayer = -1000.0f;

	setBoundingSphere( Splatterling::SplatterlingBoundingSphereSize * Splatterling::SplatterlingSizeFactor );

	initTexCoordArray();
}


Splatterling::~Splatterling()
{
	gluDeleteQuadric( mQuadric );
	delete mMaterial;
	delete mWingSound;
	delete mSnapSound;
}


static QVector3D randomPointOnWorld( World * world )
{
//	QVector3D pos( RandomNumber::minMax( -500, 500 ), 0, RandomNumber::minMax( -500, 500 ) );
	QVector3D terrainSize = world->landscape()->terrain()->size();
	QVector3D terrainOffset = world->landscape()->terrain()->offset();
	QVector3D pos;
	GLfloat distanceToPlayer = 0.0f;
	bool distanceIsValid = false;
	do
	{
		pos = QVector3D( RandomNumber::minMax( terrainOffset.x(), terrainSize.x() + terrainOffset.x() ), 0, RandomNumber::minMax( terrainOffset.z(), terrainSize.z() + terrainOffset.z() ) );
		pos.setY( world->landscape()->terrain()->getHeight( pos ) );

		distanceToPlayer = ( world->player()->position() - pos ).length();

		if( distanceToPlayer > Splatterling::DetectionDistance*2.0f && distanceToPlayer < Splatterling::DetectionDistance*5.0f)
		{
			distanceIsValid = true;
		}
	}
	while( pos.y() <= world->landscape()->waterHeight() || !distanceIsValid );

	return pos;
}


void Splatterling::randomDestinationPoint()
{
	QVector2D twoDimPosXZ;
	QVector3D pos;
	bool destinationPointValid = false;

	do{
		pos = QVector3D( RandomNumber::minMax( this->position().x() - 100, this->position().x() + 100 ), 0, RandomNumber::minMax( this->position().z() - 100, this->position().z() + 100 ) );
		twoDimPosXZ = QVector2D(pos.x(), (pos.z()/(world()->landscape()->terrain()->size().z()/2.0f))*(world()->landscape()->terrain()->size().x()/2.0f) );

		float dist = (twoDimPosXZ - QVector2D(0,0)).length();

		if( dist < (world()->landscape()->terrain()->size().x()/2.0f) ){
			destinationPointValid = true;
		}

	}while (!destinationPointValid);
	pos.setY( this->world()->landscape()->terrain()->getHeight( pos ) + RandomNumber::minMax( 20, 50 ) );
	destinationPoint = pos;
}


void Splatterling::updateSelf( const double & delta )
{
	switch( state() )
	{
		case SPAWNING:
		{
			setPosition( randomPointOnWorld( world() ) + QVector3D( 0, 20, 0 ) );
			setState( ALIVE );
			setLife( 50 );
			mHeightAboveGround = 6.0f;

			randomDestinationPoint();

			break;
		}
		case ALIVE:
		{
			mWingSound->setPositionAutoVelocity( this->worldPosition(), delta );
			mSnapSound->setPositionAutoVelocity( this->worldPosition(), delta );
			QVector3D vectPlayerSplatter = world()->player()->worldPosition() - worldPosition();
			GLfloat dist = ( vectPlayerSplatter ).length();
			vectPlayerSplatter = vectPlayerSplatter.normalized();

			double anglePlayerInSight = acos( QVector3D::dotProduct( vectPlayerSplatter, worldDirection() ) ) * 360 / M_PI;

			if( anglePlayerInSight < 100 && anglePlayerInSight > -100 && dist < 2.0f * Splatterling::DetectionDistance )
			{
				playerDetected = true;
			}
			else if( dist < Splatterling::DetectionDistance )
			{
				playerDetected = true;
			}
			else
			{
				playerDetected = false;
			}

			if( dist < 10 )
			{
				//Player in front of player
				if( recalculationOfRotationAngle )
				{
					mTarget = QVector3D( mTarget.x(), worldPosition().y(), mTarget.z() );
					mTarget = ( mTarget - worldPosition() ).normalized();
					recalculationOfRotationAngle = false;
				}

				QVector3D directionToTarget = mTarget;
				QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D( 0, 1, 0 ) );
				setRotation( QQuaternion::slerp( rotation(), targetRotation, 5 * delta ) );
				setPosition( position() + direction()*delta * 8.0 );

				if( mCoolDown == 0.0f )
				{
					world()->player()->receiveDamage( 1 );
					mCoolDown = 0.1f;
				}
			}
			else if( playerDetected )
			{
				//Player near get him
				mTarget = world()->player()->worldPosition();
				mTarget += QVector3D( 0, 1, 0 );
				QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
				QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D( 0, 1, 0 ) );
				setRotation( QQuaternion::slerp( rotation(), targetRotation, 5 * delta ) );
				setPosition( position() + direction()*delta * 12.0 );
				recalculationOfRotationAngle = true;
			}
			else
			{
				//player not in near, just move somehow
				dist = ( destinationPoint - worldPosition() ).length();
				recalculationOfRotationAngle = true;

				if( dist > 5 )
				{
					mTarget = destinationPoint;
					QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
					QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D( 0, 1, 0 ) );
					setRotation( QQuaternion::slerp( rotation(), targetRotation, 5 * delta ) );
					setPosition( position() + direction()*delta * 10.0 );
				}
				else
				{
					randomDestinationPoint();
				}
			}

			recalculateWingPosition();

			if( life() <= 0 )
			{
				world()->player()->receivePoints( 100 );
				setState( DYING );
			}
			break;
		}
		case DYING:
		{
			mWingSound->stop();
			mHeightAboveGround = 3.0f;

			mVelocityY += -10.0f * delta;	// apply some gravity
			setPositionY( position().y() + mVelocityY *delta );

			float landscapeHeight;

			if( world()->landscape()->terrain()->getHeight( position(), landscapeHeight ) )
			{
				if( landscapeHeight + mHeightAboveGround > position().y() )
				{
					setPositionY( landscapeHeight + mHeightAboveGround );
					/*if( mVelocityY < 0.0f )
						mVelocityY = 0.0f;*/
				}
			}

			if( !( position().y() > landscapeHeight + mHeightAboveGround ) )
			{
				if( mWingLeftDisintegrated || mWingRightDisintegrated )
				{
					mBodyHittedToGround = true;
					world()->splatterSystem()->spray( worldPosition(), mVelocityY*(-10.0f) );
					world()->player()->receivePoints( mVelocityY*(-10.0f) );
				}
				setState( DEAD );
			}

			break;
		}
		case DEAD:
			break;
	}

	if( mCoolDown > delta )
		mCoolDown -= delta;
	else
		mCoolDown = 0.0f;
}


void Splatterling::drawSelf()
{
	glColor4f(1,1,1,1);
	glNormal3f(0,1,0);
	mMaterial->bind();
/*
	glBindBuffer( GL_ARRAY_BUFFER, this->BufferName[COLOR_OBJECT] );
	glBufferData( GL_ARRAY_BUFFER, Splatterling::ColorSize, ColorData, GL_STREAM_DRAW );
	glColorPointer( 3, GL_UNSIGNED_BYTE, 0, 0 );
*/
	glBindBuffer( GL_ARRAY_BUFFER, this->BufferName[POSITION_OBJECT] );
	glBufferData( GL_ARRAY_BUFFER, Splatterling::PositionSize, PositionData, GL_STREAM_DRAW );
	glVertexPointer( 3, GL_FLOAT, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, this->BufferName[TEXTURE_OBJECT] );
	glBufferData( GL_ARRAY_BUFFER, Splatterling::TexSize, TextureCoordData, GL_STREAM_DRAW );
	glTexCoordPointer( 2, GL_FLOAT, 0, 0 );



	glBindBuffer( GL_ARRAY_BUFFER, this->BufferName[NORMAL_OBJECT] );
	glBufferData( GL_ARRAY_BUFFER, Splatterling::NormalSize, GlobalNormalData, GL_STREAM_DRAW );
	glNormalPointer(GL_FLOAT, 0, 0 );



	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
//	glEnableClientState( GL_COLOR_ARRAY );

//	glDrawArrays( GL_TRIANGLE_FAN, 0, Splatterling::BodyVertexCount );

	if( !mHeadDisintegrated && ! mBodyHittedToGround ){
		//body
		glDrawArrays( GL_TRIANGLE_FAN, 6, 10 );
		glDrawArrays( GL_TRIANGLE_STRIP, 16, 18 );

		//head
		glDrawArrays( GL_TRIANGLE_FAN, Splatterling::BodyVertexCount, 10 );
		glDrawArrays( GL_TRIANGLE_STRIP, Splatterling::BodyVertexCount + 10, 18 );
//		glDrawArrays( GL_TRIANGLE_STRIP, 6 + Splatterling::HeadVertexCount-4, 4 );
	}

	glDisableClientState( GL_NORMAL_ARRAY );

	//wings
	if( !mWingLeftDisintegrated )
	{
		glDrawArrays( GL_TRIANGLES, Splatterling::BodyVertexCount + Splatterling::HeadVertexCount, 3 );
	}

	if( !mWingRightDisintegrated )
	{
		glDrawArrays( GL_TRIANGLES, Splatterling::BodyVertexCount + Splatterling::HeadVertexCount + 3, 3 );
	}

//	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	mMaterial->release();
}


AObject * Splatterling::intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction, float & length, QVector3D * normal )
{
	AObject * nearestTarget = AObject::intersectLine( exclude, origin, direction, length, normal );
	float rayLength;

	if( Sphere::intersectCulledRay( worldPosition(), boundingSphereRadius(), origin, direction, &rayLength ) )
	{
		if( rayLength < length )
		{
			rayLength = length;

			bool hit = false;

			hit |= intersectHead( origin, direction, rayLength );
			hit |= intersectBody( origin, direction, rayLength );
			hit |= intersectRightWing( origin, direction, rayLength );
			hit |= intersectLeftWing( origin, direction, rayLength );

			if( hit )
			{
				if( rayLength < length )
				{
					// intersection closer than previous intersections?
					length = rayLength;

					if( normal )	// interested in normal?
						*normal = origin - worldPosition();

					nearestTarget = this;
				}
			}
		}
	}

	return nearestTarget;
}


bool Splatterling::intersectBody( const QVector3D & origin, const QVector3D & direction, float & intersectionDistance )
{
	float rayLength;

	if( !mHeadDisintegrated )
	{
		if( Intersection::intersectTriangleFan( PositionData, 6, 15, modelMatrix(), origin, direction, &rayLength ) )
		{
			if( rayLength < intersectionDistance )
			{
				intersectionDistance = rayLength;
				return true;
			}
		}

		if( Intersection::intersectTriangleStrip( PositionData, 16, BodyVertexCount - 1, modelMatrix(), origin, direction, &rayLength ) )
		{
			if( rayLength < intersectionDistance )
			{
				intersectionDistance = rayLength;
				return true;
			}
		}
	}
	return false;
}


bool Splatterling::intersectLeftWing( const QVector3D & origin, const QVector3D & direction, float & intersectionDistance )
{
	QVector3D v[3];

	float rayLength;
	bool hit = false;

	if( !mWingLeftDisintegrated )
	{
		//LeftWing
		for( int i = 0; i < 3; i++ )
		{
			v[i] = QVector3D( PositionData[( BodyVertexCount + HeadVertexCount + i ) * 3], PositionData[( BodyVertexCount + HeadVertexCount + i ) * 3 + 1], PositionData[( BodyVertexCount + HeadVertexCount + i ) * 3 + 2] );
		}
		if( Triangle::intersectCulledRay( this->pointToWorld(v[0]), this->pointToWorld(v[1]), this->pointToWorld(v[2]), origin, direction, &rayLength ) ||
			Triangle::intersectCulledRay( this->pointToWorld(v[0]), this->pointToWorld(v[2]), this->pointToWorld(v[1]), origin, direction, &rayLength ) )
		{
			if( rayLength < intersectionDistance )
			{
				intersectionDistance = rayLength;
				hit = true;
			}
		}
	}

	return hit;
}

bool Splatterling::intersectRightWing( const QVector3D & origin, const QVector3D & direction, float & intersectionDistance )
{
	QVector3D v[3];

	float rayLength;
	bool hit = false;

	if( !mWingRightDisintegrated )
	{
		//rightWing
		for( int i = 0; i < 3; i++ )
		{
			v[i] = QVector3D( PositionData[( BodyVertexCount + HeadVertexCount + 3 + i ) * 3], PositionData[( BodyVertexCount + HeadVertexCount + 3 + i ) * 3 + 1], PositionData[( BodyVertexCount + HeadVertexCount + 3 + i ) * 3 + 2] );
		}
		if( Triangle::intersectCulledRay( this->pointToWorld(v[0]), this->pointToWorld(v[1]), this->pointToWorld(v[2]), origin, direction, &rayLength ) ||
			Triangle::intersectCulledRay( this->pointToWorld(v[0]), this->pointToWorld(v[2]), this->pointToWorld(v[1]), origin, direction, &rayLength ) )
		{
			if( rayLength < intersectionDistance )
			{
				intersectionDistance = rayLength;
				hit = true;
			}
		}
	}

	return hit;
}


bool Splatterling::intersectHead( const QVector3D & origin, const QVector3D & direction, float & intersectionDistance )
{
	float rayLength;

	if(! mHeadDisintegrated )
	{
		//inner
		if( Intersection::intersectTriangleFan( PositionData, BodyVertexCount, BodyVertexCount + 9, modelMatrix(), origin, direction, &rayLength ) )
		{
			if( rayLength < intersectionDistance )
			{
				intersectionDistance = rayLength;
				return true;
			}
		}

		//Outter
		if( Intersection::intersectTriangleStrip( PositionData, BodyVertexCount + 10, BodyVertexCount + HeadVertexCount - 5, modelMatrix(), origin, direction, &rayLength ) )
		{
			if( rayLength < intersectionDistance )
			{
				intersectionDistance = rayLength;
				return true;
			}
		}
	}

	return false;
}


void Splatterling::receiveDamage( int damage, const QVector3D * position, const QVector3D * direction )
{
	float rayLength = FLT_MAX;
	int targetBodyPart = TARGET_BODY;

	QVector3D mTrailStart = ( *position - (*direction) * 0.01f );

	if( intersectBody( mTrailStart, *direction, rayLength ) )
		targetBodyPart = TARGET_BODY;

	if( intersectRightWing( mTrailStart, *direction, rayLength ) )
		targetBodyPart = TARGET_WING_RIGHT;

	if( intersectLeftWing( mTrailStart, *direction, rayLength ) )
		targetBodyPart = TARGET_WING_LEFT;

	if( intersectHead( mTrailStart, *direction, rayLength ) )
		targetBodyPart = TARGET_HEAD;

	damage *= damageMultiplicationFactor[targetBodyPart];
	ACreature::receiveDamage( damage, direction );
	mDamageOnBodyPart[targetBodyPart] += damage;

	if( targetBodyPart == TARGET_WING_LEFT )
	{
		if( mDamageOnBodyPart[TARGET_WING_LEFT] >= 5.0f )
		{
			mWingLeftDisintegrated = true;
			setLife( 0 );
			setState( DYING );
		}
	}
	else if( targetBodyPart == TARGET_WING_RIGHT )
	{
		if( mDamageOnBodyPart[TARGET_WING_RIGHT] >= 5.0f )
		{
			mWingRightDisintegrated = true;
			setLife( 0 );
			setState( DYING );
		}
	}
	else if( targetBodyPart == TARGET_HEAD )
	{
		if( mDamageOnBodyPart[TARGET_HEAD] >= 5.0f )
		{
			mHeadDisintegrated = true;
			setLife( 0 );
			world()->player()->receivePoints( 200 );
			setState( DYING );
		}
	}

	QVector3D splatterSource;

	if( position )
		splatterSource = *position;
	else
		splatterSource = worldPosition();

	if( state() != DEAD )
		world()->splatterSystem()->spray( splatterSource, damage );
	else
		world()->splatterSystem()->spray( splatterSource, damage / 2.0f );
}


void Splatterling::recalculateWingPosition()
{
	if( wingUpMovement )
	{
		PositionData[Splatterling::WingOneYPos]   += 0.02f;
		PositionData[Splatterling::WingOneYPos + 3] += 0.03f;
		PositionData[Splatterling::WingTwoYPos]   += 0.02f;
		PositionData[Splatterling::WingTwoYPos + 3] += 0.03f;

		if( PositionData[Splatterling::WingOneYPos] >= 4.0f )
		{
			wingUpMovement = false;
		}
	}
	else
	{
		PositionData[Splatterling::WingOneYPos]   -= 0.02f;
		PositionData[Splatterling::WingOneYPos + 3] -= 0.03f;
		PositionData[Splatterling::WingTwoYPos]   -= 0.02f;
		PositionData[Splatterling::WingTwoYPos + 3] -= 0.03f;

		if( PositionData[Splatterling::WingOneYPos] <= 0.0f )
		{
			wingUpMovement = true;
		}
	}
}
