#include "Dummy.hpp"

#include <scene/object/World.hpp>
#include <geometry/SplatterSystem.hpp>
#include <utility/random.hpp>
#include <utility/quaternion.hpp>

#include <math.h>
#include <float.h>


Dummy::Dummy( World * world ) : ACreature( world )
{
	mQuadric = gluNewQuadric();
	mVelocityY = 0.0f;
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
		case SPAWNING:
		{
			setPosition( randomPointOnWorld( world() ) + QVector3D(0,10,0) );
			setState( ALIVE );
			setLife( 100 );
			break;
		}

		case ALIVE:
		{
			mTarget = world()->teapot()->worldPosition();
			QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
			QQuaternion targetRotation = quaternionLookAt( directionToTarget, QVector3D(0,1,0) );
			setRotation( QQuaternion::slerp( rotation(), targetRotation, 0.95 ) );
			setPosition( position() + direction()*delta*10.0 );
			if( life() <= 0 )
				setState( DYING );
			break;
		}

		case DYING:
			setState( DEAD );
			break;

		case DEAD:
			break;
	}
	mVelocityY += -80.0f * delta;	// apply gravity
	setPositionY( position().y() + mVelocityY * delta );
	float landscapeHeight;
	if( world()->landscape()->terrain()->getHeight( position(), landscapeHeight ) )
	{
		if( landscapeHeight + 5.0f > position().y() )
		{
			setPositionY( landscapeHeight + 5.0f );
			if( mVelocityY < 0.0f )
				mVelocityY = 0.0f;
		}
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


static bool getLineSphereIntersection( const QVector3D & sphereCenter, const float & sphereRadius,
	const QVector3D & rayOrigin, const QVector3D & rayDirection,
	float & length, QVector3D * normal = NULL )
{
	QVector3D relRayOrigin = rayOrigin - sphereCenter;

	//Compute A, B and C coefficients
	float a = rayDirection.lengthSquared();
	float b = 2.0f * QVector3D::dotProduct( rayDirection, relRayOrigin );
	float c = relRayOrigin.lengthSquared() - (sphereRadius * sphereRadius);

	//Find discriminant
	float disc = b * b - 4 * a * c;

	// if discriminant is negative there are no real roots, so return false as ray misses sphere
	if( disc < 0.0f )
		return false;

	float distSqrt = sqrtf( disc );
	float q;
	if( b < 0.0f )
		q = (-b - distSqrt)/2.0f;
	else
		q = (-b + distSqrt)/2.0f;

	// compute t0 and t1
	float t0 = q / a;
	float t1 = c / q;

	// make sure t0 is smaller than t1
	if( t0 > t1 )
	{
		// if t0 is bigger than t1 swap them around
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	// if t1 is less than zero, the object is in the ray's negative direction
	// and consequently the ray misses the sphere
	if( t1 < 0.0f )
		return false;

	// if t0 is less than zero, the intersection point is at t1
	if( t0 < 0.0f )
	{
		qDebug() << "Baeam1!";
		if( length > t1 )
			length = t1;
		return true;
	}
	else	// else the intersection point is at t0
	{
		qDebug() << "Baeam0!";
		if( length > t0 )
			length = t0;
		return true;
	}
}


AObject * Dummy::getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & length, QVector3D * normal )
{
	AObject * nearestTarget = AObject::getLineIntersection( origin, direction, length, normal );
	if( getLineSphereIntersection( worldPosition(), 4, origin, direction, length, normal ) )
		nearestTarget = this;
	return nearestTarget;
}


void Dummy::receiveDamage( int damage, const QVector3D * position, const QVector3D * direction )
{
	ACreature::receiveDamage( damage, direction );
	QVector3D splatterSource;
	if( position )
		splatterSource = *position;
	else
		splatterSource = worldPosition();
	world()->splatterSystem()->spray( splatterSource, 30.0f );
}
