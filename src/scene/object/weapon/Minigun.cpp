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

#include "Minigun.hpp"

#include "../World.hpp"

#include <geometry/ParticleSystem.hpp>
#include <scene/Scene.hpp>
#include <utility/Quaternion.hpp>
#include <resource/AudioSample.hpp>
#include <resource/Material.hpp>


Minigun::Minigun( World * world ) :
	AWeapon( world, 10000, 200, 200 )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mName = "Minigun";
	mDrawn = false;
	mReload = false;
	mCoolDown = 0.1f;
	mFired = false;
	mRange = 100.0f;
	mTrailRadius = 0.01f;
	mDamage = 1.0f;
	mRotation = 0.0f;
	mMaterial = new Material( scene()->glWidget(), "BlackSteel" );
	mFireSound = new AudioSample( "minigun" );
	mFireSound->setLooping( true );

	mImpactParticleMaterial = new Material( scene()->glWidget(), "DirtParticle" );
	mImpactParticles = new ParticleSystem( 128 );
	mImpactParticles->setSize( 0.5f );
	mImpactParticles->setGravity( QVector3D( 0.0f, -80.0f, 0.0f ) );
	mImpactParticles->setDrag( 0.25f );
	mImpactParticles->setMinLife( 1.0f );
	mImpactParticles->setMaxLife( 2.0f );
}


Minigun::~Minigun()
{
	gluDeleteQuadric( mQuadric );
	delete mImpactParticleMaterial;
	delete mImpactParticles;
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
	mRPM = 0.0f;
}


void Minigun::holster()
{
	mDrawn = false;
	mFired = false;
	mFireSound->stop();
}


void Minigun::reload()
{
	mReload = true;
	mCoolDown = 5.0f;
	mFireSound->stop();
}


void Minigun::setTarget( const QVector3D * target )
{
	mTarget = target;
}


void Minigun::updateSelf( const double & delta )
{
	mImpactParticles->update( delta );
	if( mDrawn )
	{
		setRotation( QQuaternion::slerp( rotation(), getRotationToTarget( mTarget, 0.7f ), 2.0 * delta ) );
		if( mReload )
		{
			if( mCoolDown <= 0.0f )
			{
				reloadClip();
				mReload = false;
			}
			spinDown( delta );
		}
		else if( mFired )
		{
			if( mClipAmmo == 0 )
			{
				mFireSound->stop();
			}
			else if( mCoolDown <= 0.0f && mRPM >= 600.0f )
			{
				mTrailStart = worldPosition();
				mTrailDirection = worldDirection();
				mTrailLength = mRange;
				AObject * target = world()->intersectLine( this, mTrailStart, mTrailDirection, mTrailLength );
				mTrailEnd = mTrailStart + mTrailDirection*mTrailLength;

				if( target )
					mImpactParticles->emitSpherical( mTrailEnd, 16, 5.0, 10.0, QVector3D(0,10,0) );
				ACreature * victim = dynamic_cast<ACreature*>(target);
				if( victim )
					victim->receiveDamage( mDamage, &mTrailEnd, &mTrailDirection );

				if( !mFireSound->isPlaying() )
				{
					mFireSound->play();
				}
				mClipAmmo--;
				mCoolDown = 0.1f;
			}

			spinUp( delta );
		}
		else
		{
			spinDown( delta );
		}
	}

	mRotation += mRPM * delta;

	if( mCoolDown > delta )
		mCoolDown -= delta;
	else
		mCoolDown = 0.0f;
}


void Minigun::drawSelf()
{
	if( !mDrawn )
		return;

	mMaterial->bind();

	glPushMatrix();

	glTranslatef( 0.0f, 0.0f, -0.4f );
	glRotatef( mRotation, 0.0f, 0.0f, 1.0f );

	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -0.0f );
	gluCylinder( mQuadric, 0.08f, 0.08f, 0.3f, 16, 1 );

	glTranslatef( 0.0f, 0.0f, 0.5f );
	gluCylinder( mQuadric, 0.08f, 0.08f, 0.02f, 16, 1 );
	gluDisk( mQuadric, 0.0f, 0.08f, 16, 1 );

	glTranslatef( 0.0f, 0.0f, 0.1f );
	gluCylinder( mQuadric, 0.08f, 0.08f, 0.15f, 16, 1 );
	gluDisk( mQuadric, 0.0f, 0.08f, 16, 1 );

	glPopMatrix();

	for( int i=0; i<=360; i+=60 )
	{
		glPushMatrix();
		glRotatef( i, 0.0f, 0.0f, 1.0f );
		glTranslatef( 0.05f, 0.0f, 0.0f );
		gluCylinder( mQuadric, 0.02f, 0.02f, 0.6f, 16, 1 );
		glPopMatrix();
	}

	mMaterial->release();

	glPopMatrix();
}


void Minigun::draw2Self()
{
	glPushMatrix();
	glLoadMatrix( world()->modelViewMatrix() );

	// particles on impact
	glEnable( GL_TEXTURE_2D );
	mImpactParticleMaterial->bind();
	mImpactParticles->draw( world()->modelViewMatrix() );
	mImpactParticleMaterial->release();

	glPopMatrix();
}


void Minigun::spinUp( const double & delta )
{
	if( mRPM < 600.0f )
	{
		mRPM += delta * 300;
	}
	else
	{
		mRPM = 600.0f;
	}
}


void Minigun::spinDown( const double & delta )
{
	if( mRPM >= 0.1f )
	{
		mRPM -= delta * 300;
	}
	else
	{
		mRPM = 0.0f;
	}

	mFireSound->stop();
}


void Minigun::reloadClip()
{
	if( mClipAmmo < mClipSize )
	{
		if( mAmmo > mClipSize )
		{
			mClipAmmo = mClipSize;
			mAmmo -= mClipSize;
		}
		else
		{
			mClipAmmo = mAmmo;
			mAmmo -= mAmmo;
		}
	}
}
