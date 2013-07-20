#include "Eye.hpp"

#include <scene/Scene.hpp>

#include <utility/glWrappers.hpp>
#include <utility/alWrappers.hpp>


Eye::Eye( Scene * scene ) :
	mScene( scene ),
	mPosition(),
	mRotation(),
	mScale( 1, 1, 1 ),
	mFOV( 60.0f ),
	mNearPlane( 0.1f ),
	mFarPlane( 500.0f )
{
}


Eye::Eye( Eye & other ) :
	mScene( other.mScene ),
	mPosition( other.mPosition ),
	mRotation( other.mRotation ),
	mScale( other.mScale ),
	mFOV( other.mFOV ),
	mNearPlane( other.mNearPlane ),
	mFarPlane( other.mFarPlane )
{
}


Eye::~Eye()
{
}


void Eye::applyAL( const double & delta )
{
	QVector3D up = mRotation.rotatedVector( QVector3D(0,1,0) );
	QVector3D direction = mRotation.rotatedVector( QVector3D(0,0,1) );
	ALfloat listenerOri[] = {
		(ALfloat)direction.x(), (ALfloat)direction.y(), (ALfloat)direction.z(),
		(ALfloat)up.x(), (ALfloat)up.y(), (ALfloat)up.z()
	};
	alListener( AL_POSITION, mPosition );
	alListenerv( AL_ORIENTATION, listenerOri );
	QVector3D velocity = (mPosition - mLastPosition) / delta;
	mLastPosition = mPosition;
	alListener( AL_VELOCITY, velocity );
}


void Eye::applyGL()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( mFOV, (float)mScene->width()/mScene->height(), mNearPlane, mFarPlane );

	if( !mAttached.isNull() )
	{
		mPosition = mAttached.data()->position();
		mRotation = mAttached.data()->rotation();
	}

	mMatrixInverse.setToIdentity();
	mMatrixInverse.translate( mPosition );
	mMatrixInverse.rotate( mRotation );
	mMatrixInverse.rotate( 180.0f, QVector3D( 0, 1, 0 ) );
	mMatrixInverse.scale( mScale );
	mMatrix = mMatrixInverse.inverted();

	glMatrixMode( GL_MODELVIEW );
	glLoadMatrix( mMatrix );

	applyClippingPlanes();
}


void Eye::setClippingPlane( int n, QVector4D plane )
{
	if( plane.isNull() )
	{
		glDisable( GL_CLIP_PLANE0 + n );
		mClippingPlanes.remove( n );
	} else {
		mClippingPlanes[n] = plane;
	}
}


void Eye::applyClippingPlanes()
{
	QMap<int,QVector4D>::const_iterator i = mClippingPlanes.constBegin();
	while( i != mClippingPlanes.constEnd() )
	{
		double plane[4]={ i.value().x(), i.value().y(), i.value().z(), i.value().w() };
		glClipPlane( GL_CLIP_PLANE0 + i.key(), plane );
		++i;
	}
}


void Eye::enableClippingPlanes()
{
	QMap<int,QVector4D>::const_iterator i = mClippingPlanes.constBegin();
	while( i != mClippingPlanes.constEnd() )
	{
		glEnable( GL_CLIP_PLANE0 + i.key() );
		++i;
	}
}


void Eye::disableClippingPlanes()
{
	QMap<int,QVector4D>::const_iterator i = mClippingPlanes.constBegin();
	while( i != mClippingPlanes.constEnd() )
	{
		glDisable( GL_CLIP_PLANE0 + i.key() );
		++i;
	}
}


void Eye::attach( QWeakPointer< AObject > object )
{
	mAttached = object;
}


void Eye::detach()
{
	mAttached = QWeakPointer<AObject>();
}
