#include "Laser.hpp"

#include <geometry/SplatterSystem.hpp>
#include <scene/object/World.hpp>
#include <scene/Scene.hpp>


Laser::Laser( Scene * scene, World * world ) :
	AWeapon( scene ),
	mWorld( world )
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
			QVector3D normal;
			if( mWorld->getLineIntersection( mTrailStart, mTrailDirection, mTrailLength, normal ) )
			{
				mTrailEnd = mTrailStart + mTrailDirection*mTrailLength;
				mWorld->splatterSystem()->spray( mTrailEnd + QVector3D(0,1.0f,0), 30.0f );
			} else {
				mTrailEnd = mTrailStart + mTrailDirection*mRange;
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
	gluCylinder( mQuadric, 0.1f, 0.1f, 0.4f, 16, 16 );
}


void Laser::drawAfterSelf()
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

	glPushMatrix();
	glLoadMatrix( scene()->eye()->matrix() );
	glBegin( GL_TRIANGLE_STRIP );
		glVertex(-crossDir*mTrailRadius + mTrailStart);
		glVertex( crossDir*mTrailRadius + mTrailStart);
		glVertex(-crossDir*mTrailRadius + mTrailEnd );
		glVertex( crossDir*mTrailRadius + mTrailEnd );
	glEnd();
	glPopMatrix();
	
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}
