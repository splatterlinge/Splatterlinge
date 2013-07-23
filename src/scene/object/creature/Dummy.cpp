#include "Dummy.hpp"

#include <scene/object/World.hpp>
#include <utility/random.hpp>
#include <utility/quaternion.hpp>

#include <math.h>
#include <float.h>


Dummy::Dummy( World * world ) : ACreature( world )
{
	mQuadric = gluNewQuadric();
	mTimeToOrientationChange = 1.0f;
}


Dummy::~Dummy()
{
	gluDeleteQuadric( mQuadric );
}


static QVector3D randomPointOnWorld( World * world )
{
	QVector3D pos( randomMinMax(-100,100), 0, randomMinMax(-100,100) );
	pos.setY( world->landscape()->terrain()->getHeight( pos ) );
	return pos;
}


void Dummy::updateSelf( const double & delta )
{
	switch( state() )
	{
		case ACreature::SPAWNING:
		{
			setPosition( randomPointOnWorld( world() ) + QVector3D(0,10,0) );
			setState( ACreature::ALIVE );
			break;
		}

		case ACreature::ALIVE:
		{
			mTimeToOrientationChange -= delta;
			if( mTimeToOrientationChange < 0.0 )
			{
				mTimeToOrientationChange = randomMinMax( 1, 2 );
//				mTarget = randomPointOnWorld( world() ) + QVector3D(0,20,0);
			}
			mTarget = world()->player()->teapot()->worldPosition();
			QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
			setRotation( QQuaternion::slerp( rotation(), quaternionLookAt( directionToTarget, QVector3D(0,1,0) ), 0.05 ) );
			setPosition( position() + direction()*delta*10.0 );
			break;
		}
		case ACreature::DYING:
			break;

		case ACreature::DEAD:
			break;
	}
}


void Dummy::drawSelf()
{
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LIGHTING );

	glMaterial( GL_FRONT_AND_BACK, GL_AMBIENT, QVector4D(0,0,0,0) );
	glMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE, QVector4D(1,1,1,1) );
	glMaterial( GL_FRONT_AND_BACK, GL_SPECULAR, QVector4D(1,1,1,1) );
	glMaterial( GL_FRONT_AND_BACK, GL_EMISSION, QVector4D(0,0,0,0) );
	glMaterial( GL_FRONT_AND_BACK, GL_SHININESS, 32.0f );

	gluSphere( mQuadric, 4, 8, 8 );

	glDisable( GL_LIGHTING );
	glBegin( GL_LINES );
		glColor(0.0,0.0,1.0);
		glVertex(0,0,0);
		glVertex(0,0,10);

		glColor(0.0,1.0,0.0);
		glVertex(0,0,0);
		glVertex(0,10,0);

		glColor(1.0,0.0,0.0);
		glVertex(0,0,0);
		glVertex(10,0,0);
	glEnd();
}


void Dummy::receiveDamage( int damage, const QVector3D & direction )
{
	ACreature::receiveDamage( damage, direction );
}
