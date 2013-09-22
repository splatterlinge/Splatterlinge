#include "Knife.hpp"


#include <resource/StaticModel.hpp>


Knife::Knife( World * world ) :
	AWeapon( world, -1, -1, -1 )
{
	mModel = new StaticModel( world->scene()->glWidget(), "knife" );

	mFired = false;
	mHitting = false;
	mRotation = 0.0f;
	mDirection = 0.0f;
	mCooldown = 0.0f;
	mRange = 10.0f;
	mDamage = 200.0f;
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
			if( mRotation < -3.0f )
			{
				mDirection = -mDirection;
			}

			if( mDirection < 0.0f )
				mRotation += mDirection*delta*20;
			else
				mRotation += mDirection*delta*30;

			if( mRotation > 0.0f )
			{
				mHitting = false;

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

	glScalef( 0.02,0.02,0.02 );
	glTranslatef( 30,-250-mRotation*5,-40+mRotation*20 );
	glRotatef( mRotation, 1.0f, 0.0f, 0.0f );
	mModel->draw();

	glPopMatrix();
}
