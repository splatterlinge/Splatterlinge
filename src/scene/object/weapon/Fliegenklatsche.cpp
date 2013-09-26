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

#include "Fliegenklatsche.hpp"

#include <resource/StaticModel.hpp>
#include <resource/AudioSample.hpp>


Fliegenklatsche::Fliegenklatsche( World * world ) :
	AWeapon( world, -1, -1, -1 )
{
	mModel = new StaticModel( world->scene()->glWidget(), "fliegenklatsche" );

	mName = "Fliegenklatsche";
	mFired = false;
	mHit = false;
	mHitting = false;
	mRotation = 0.0f;
	mStep = 0.0f;
	mCooldown = 0.0f;
	mRange = 10.0f;
	mDamage = 20.0f;

	mHitSound = new AudioSample( "flyswatter" );
	mHitSound->setLooping( false );
}


Fliegenklatsche::~Fliegenklatsche()
{
	delete mModel;
	delete mHitSound;
}

void Fliegenklatsche::triggerPressed()
{
	mFired = true;
}


void Fliegenklatsche::triggerReleased()
{
	mFired = false;
}


void Fliegenklatsche::holster()
{
	mDrawn = false;
}


void Fliegenklatsche::pull()
{
	mDrawn = true;
}


void Fliegenklatsche::updateSelf( const double &delta )
{
	if( mDrawn )
	{
		if( mHitting )
		{
			mStep += delta*4;
			mRotation = (pow(mStep, 4)*0.23f + pow(mStep, 3)*(-2) + pow(mStep, 2)*4.5f + mStep*(-1));

			if( mStep > 3.21f && !mHit )
			{
				mHitSound->play();

				mTrailStart = world()->player()->position();
				mTrailDirection = world()->player()->direction();
				mTrailLength = mRange;
				AObject * target = world()->intersectLine( this, mTrailStart, mTrailDirection, mTrailLength );
				mTrailEnd = mTrailStart + mTrailDirection*mTrailLength;
				ACreature * victim = dynamic_cast<ACreature*>(target);
				if( victim )
				{
					victim->receiveDamage( mDamage, &mTrailEnd, &mTrailDirection );
				}

				mHit = true;
			}

			if( mStep > 4.84f )
			{
				mHitting = false;
			}
		}
		else if( mFired && mCooldown <= 0.0f )
		{
			mHitting = true;
			mHit = false;
			mStep = 0.0f;
			mRotation = 0.0f;
			mCooldown = 1.0f;
		}
	}

	if( mCooldown > delta )
		mCooldown -= delta;
	else
		mCooldown = 0.0f;
}


void Fliegenklatsche::drawSelf()
{
	if( !mDrawn )
		return;

	glPushMatrix();

	glScalef( 0.1, 0.1, 0.1 );
	glTranslatef( 7, -2, 0 );
	glRotatef( -mRotation*30, 1.0f, 0.0f, 0.0f );
	glRotatef( 5.0f-mRotation, 0.0f, 1.0f, 0.0f );
	mModel->draw();

	glPopMatrix();
}
