#include "Laser.hpp"

#include "../World.hpp"

#include <geometry/SplatterSystem.hpp>
#include <scene/Scene.hpp>


Laser::Laser( World * world ) :
	AWeapon( world )
{
	mQuadric = gluNewQuadric();
	mCoolDown = 0.0f;
	mTrailAlpha = 0.0f;
	mFired = false;
	mRange = 250.0f;
	mTrailRadius = 0.04;
}


Laser::~Laser()
{
	gluDeleteQuadric( mQuadric );
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
			mCoolDown = 1.0f;
			mTrailAlpha = 1.0f;
			mTrailStart = worldPosition();
			mTrailDirection = worldDirection();
			mTrailLength = mRange;
			AObject * target = world()->intersectLine( mTrailStart, mTrailDirection, mTrailLength );
			mTrailEnd = mTrailStart + mTrailDirection*mTrailLength;
			ACreature * victim = dynamic_cast<ACreature*>(target);
			if( victim )
			{
				victim->receiveDamage( 1, &mTrailEnd, &mTrailDirection );
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
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LIGHTING );

	glMaterial( GL_FRONT_AND_BACK, GL_AMBIENT, QVector4D(0,0,0,0) );
	glMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE, QVector4D(1,1,1,1) );
	glMaterial( GL_FRONT_AND_BACK, GL_SPECULAR, QVector4D(1,1,1,1) );
	glMaterial( GL_FRONT_AND_BACK, GL_EMISSION, QVector4D(0,0,0,0) );
	glMaterial( GL_FRONT_AND_BACK, GL_SHININESS, 32.0f );

	gluCylinder( mQuadric, 0.1f, 0.1f, 0.4f, 16, 16 );
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

	QVector3D toEye = scene()->eye()->position() - mTrailStart;
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
