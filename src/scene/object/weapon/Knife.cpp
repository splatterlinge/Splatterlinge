#include "Knife.hpp"


#include <resource/StaticModel.hpp>


Knife::Knife( World * world ) :
	AWeapon( world, -1, -1, -1 )
{
	mModel = new StaticModel( world->scene()->glWidget(), "knife" );

	mName = "Knife";
	mFired = false;
	mHitting = false;
	mRotation = 0.0f;
	mDirection = 0.0f;
	mCooldown = 0.0f;
	mRange = 10.0f;
	mDamage = 100.0f;
}


Knife::~Knife()
{
	delete mModel;
}

void Knife::triggerPressed()
{
	mFired = true;
}


void Knife::triggerReleased()
{
	mFired = false;
}


void Knife::holster()
{
	mDrawn = false;
}


void Knife::pull()
{
	mDrawn = true;
}


void Knife::updateSelf( const double &delta )
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


void Knife::drawSelf()
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
