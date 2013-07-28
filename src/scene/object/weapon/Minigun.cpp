#include "Minigun.hpp"

#include "../World.hpp"

#include <effect/SplatterSystem.hpp>
#include <scene/Scene.hpp>


Minigun::Minigun( World * world ) :
	AWeapon( world )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mCoolDown = 0.0f;
	mTrailAlpha = 0.0f;
	mFired = false;
	mRange = 100.0f;
	mTrailRadius = 0.01f;
	mDamage = 5.0f;
	mMaterial = new Material( scene()->glWidget(), "BlackSteel" );
	mFireSound = new AudioSample( "./data/sound/minigun.ogg" );
	mFireSound->setLooping( true );
}


Minigun::~Minigun()
{
	gluDeleteQuadric( mQuadric );
	delete mMaterial;
}


void Minigun::triggerPressed()
{
	mFired = true;
}


void Minigun::triggerReleased()
{
	mFired = false;
}


void Minigun::updateSelf( const double & delta )
{
	if( mFired )
	{
		if( mCoolDown <= 0.0f )
		{
			mCoolDown = 0.1f;
			mTrailAlpha = 1.0f;
			mTrailStart = worldPosition();
			mTrailDirection = worldDirection();
			mTrailLength = mRange;
			AObject * target = world()->intersectLine( this, mTrailStart, mTrailDirection, mTrailLength );
			mTrailEnd = mTrailStart + mTrailDirection*mTrailLength;
			ACreature * victim = dynamic_cast<ACreature*>(target);
			if( victim )
			{
				if( victim->life() != 0 )
				{
					victim->receiveDamage( mDamage, &mTrailEnd, &mTrailDirection );
				}
			}
		}

		if( mRotation == 0.0f )
		{
			mRotation = 0.1f;
		}
		else if( mRotation < 60.0f )
		{
			mRotation *= 1.2f;
		}
		else
		{
			mRotation += 25.0f;

			qDebug() << "fire";
			if( !mFireSound->isPlaying() )
			{
				mFireSound->play();
			}
		}
	}
	else
	{
		if( mRotation >= 0.1f )
		{
			mRotation *= 0.8f;
		}
		else
		{
			mRotation = 0.0f;
			mFireSound->stop();
		}
	}

	if( mCoolDown > delta )
		mCoolDown -= delta * 0.5;
	else
		mCoolDown = 0.0f;

	if( mTrailAlpha > delta )
		mTrailAlpha -= delta * 2.0;
	else
		mTrailAlpha = 0.0f;
}


void Minigun::drawSelf()
{
	mMaterial->bind();

	glPushMatrix();

	glColor3f( 0.7f, 0.7f, 0.7f );

	glTranslatef( 0.05f, 0.0f, 0.0f );
	glRotatef( mRotation, 0.0f, 0.0f, 1.0f );

	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, 0.0f );
	gluCylinder( mQuadric, 0.08f, 0.08f, 0.25f, 16, 1 );

	glTranslatef( 0.0f, 0.0f, 0.65f );
	gluCylinder( mQuadric, 0.08f, 0.08f, 0.05f, 16, 1 );
	gluDisk( mQuadric, 0.0f, 0.08f, 16, 1 );

	glTranslatef( 0.0f, 0.0f, 0.2f );
	gluCylinder( mQuadric, 0.08f, 0.08f, 0.05f, 16, 1 );
	gluDisk( mQuadric, 0.0f, 0.08f, 16, 1 );
	glPopMatrix();

	for( int i=0; i<=360; i+=60 )
	{
		glPushMatrix();
		glRotatef( i, 0.0f, 0.0f, 1.0f );
		glTranslatef( 0.05f, 0.0f, 0.0f );
		gluCylinder( mQuadric, 0.02f, 0.02f, 1.0f, 16, 1 );
		glPopMatrix();
	}

	glPopMatrix();

	mMaterial->release();
}


void Minigun::draw2Self()
{
	// TODO
}
