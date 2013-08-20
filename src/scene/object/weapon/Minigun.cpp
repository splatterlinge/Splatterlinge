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

#include "Minigun.hpp"

#include "../World.hpp"

#include <effect/SplatterSystem.hpp>
#include <scene/Scene.hpp>


Minigun::Minigun( World * world ) :
	AWeapon( world, 10000, 200, 200 )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mName = "Minigun";
	mDrawn = false;
	mCoolDown = 0.1f;
	mTrailAlpha = 0.0f;
	mFired = false;
	mRange = 100.0f;
	mTrailRadius = 0.01f;
	mDamage = 5.0f;
	mRotation = 0.0f;
	mMaterial = new Material( scene()->glWidget(), "BlackSteel" );
	mFireSound = new AudioSample( "./data/sound/minigun.ogg" );
	mFireSound->setLooping( true );
}


Minigun::~Minigun()
{
	gluDeleteQuadric( mQuadric );
	delete mMaterial;
	delete mFireSound;
}


void Minigun::triggerPressed()
{
	mFired = true;
}


void Minigun::triggerReleased()
{
	mFired = false;
}


void Minigun::pull()
{
	mDrawn = true;
}


void Minigun::holster()
{
	mDrawn = false;
}


void Minigun::updateSelf( const double & delta )
{
	if( mFired )
	{
		if( mCoolDown <= 0.0f && mAmmoClip > 0 )
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
				victim->receiveDamage( mDamage, &mTrailEnd, &mTrailDirection );
			}

			if( !mFireSound->isPlaying() )
			{
				mFireSound->play();
			}
			mAmmoClip--;
		}
		else
		{
			mFireSound->stop();
		}

		if( mRPM < 6000.0f )
		{
			mRPM += (mRPM+1) * delta * 7;
			mCoolDown = 0.1f;
		}
		else
		{
			mRPM = 6000.0f;
			mCoolDown = 0.0f;
		}
	}
	else
	{
		if( mRPM >= 0.1f )
		{
			mRPM -= mRPM * delta * 3;
		}
		else
		{
			mRPM = 0.0f;
		}

		mCoolDown = 0.1f;
		mFireSound->stop();
	}

	mRotation += mRPM * delta;

	if( mTrailAlpha > delta )
		mTrailAlpha -= delta * 2.0;
	else
		mTrailAlpha = 0.0f;
}


void Minigun::drawSelf()
{
	if( !mDrawn )
		return;

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
