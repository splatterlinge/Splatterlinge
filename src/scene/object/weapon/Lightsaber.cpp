#include "Lightsaber.hpp"


#include <resource/StaticModel.hpp>


Lightsaber::Lightsaber( World * world ) :
	AWeapon( world, -1, -1, -1 )
{
	mModel = new StaticModel( world->scene()->glWidget(), "lightsaber" );

	mName = "Lightsaber";
	mFired = false;
	mHitting = false;
	mRotation = 0.0f;
	mDirection = 0.0f;
	mCooldown = 0.0f;
	mRange = 10.0f;
	mDamage = 2000.0f;
}


Lightsaber::~Lightsaber()
{
	delete mModel;
}

void Lightsaber::triggerPressed()
{
	mFired = true;
}


void Lightsaber::triggerReleased()
{
	mFired = false;
}


void Lightsaber::holster()
{
	mDrawn = false;
}


void Lightsaber::pull()
{
	mDrawn = true;
}


void Lightsaber::updateSelf( const double &delta )
{
	if( mDrawn )
	{
		if( mHitting )
		{
			if( mRotation < -60.0f )
			{
				mDirection = -mDirection;
			}

			if( mDirection < 0.0f )
				mRotation += mDirection*delta*300;
			else
				mRotation += mDirection*delta*400;

			if( mRotation > 0.0f )
			{
				mHitting = false;

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
			}
		}
		else if( mFired && mCooldown <= 0.0f )
		{
			mHitting = true;
			mRotation = 0;
			mDirection = -1.0;
			mCooldown = 1.0f;
		}
	}

	if( mCooldown > delta )
		mCooldown -= delta;
	else
		mCooldown = 0.0f;
}


void Lightsaber::drawSelf()
{
	if( !mDrawn )
		return;

	glPushMatrix();

	glScalef( 0.1, 0.1, 0.1 );
	glTranslatef( 5, -2, 0 );
	glRotatef( mRotation, 1.0f, 0.0f, 0.0f );
	mModel->draw();

	glPopMatrix();
}
