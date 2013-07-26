#include "Laser.hpp"

#include "../World.hpp"

#include <effect/SplatterSystem.hpp>
#include <scene/Scene.hpp>


Laser::Laser( World * world ) :
	AWeapon( world )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mCoolDown = 0.0f;
	mTrailAlpha = 0.0f;
	mFired = false;
	mRange = 250.0f;
	mTrailRadius = 0.04f;
	mDamage = 5.0f;
	mMaterial = new Material( scene()->glWidget(), "KirksEntry" );
}


Laser::~Laser()
{
	gluDeleteQuadric( mQuadric );
	delete mMaterial;
}


void Laser::triggerPressed()
{
	mFired = true;
}


void Laser::triggerReleased()
{
	mFired = false;
}


void Laser::updateSelf( const double & delta )
{
	if( mFired )
	{
		if( mCoolDown <= 0.0f )
		{
			mCoolDown = 0.0f;
			mTrailAlpha = 1.0f;
			mTrailStart = worldPosition();
			mTrailDirection = worldDirection();
			mTrailLength = mRange;
			AObject * target = world()->intersectLine( mTrailStart, mTrailDirection, mTrailLength );
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


void Laser::drawSelf()
{
	mMaterial->bind();
	gluCylinder( mQuadric, 0.1f, 0.1f, 0.4f, 16, 16 );
	mMaterial->release();
}


void Laser::draw2Self()
{
	glColor4f( 0.2, 0.4, 1.0f, mTrailAlpha );

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );

	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	QVector3D toEye = scene()->eye()->position() - worldPosition();
	QVector3D crossDir = QVector3D::crossProduct( mTrailDirection, toEye ).normalized();

	glLoadMatrix( scene()->eye()->viewMatrix() );
	glBegin( GL_TRIANGLE_STRIP );
		glVertex(-crossDir*mTrailRadius + mTrailStart);
		glVertex( crossDir*mTrailRadius + mTrailStart);
		glVertex(-crossDir*mTrailRadius + mTrailEnd );
		glVertex( crossDir*mTrailRadius + mTrailEnd );
	glEnd();

	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}
