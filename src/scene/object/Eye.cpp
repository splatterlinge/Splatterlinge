#include "Eye.hpp"

#include "../Scene.hpp"

#include <GLWidget.hpp>


Eye::Eye( Scene * scene ) :
	AObject( scene ),
	mFOV(60.0f),
	mNearPlane(0.1f),
	mFarPlane(500.0f)
{
}


Eye::Eye( Eye & other ) :
	AObject(other),
	mFOV(other.mFOV),
	mNearPlane(other.mNearPlane),
	mFarPlane(other.mFarPlane)
{
}


Eye::~Eye()
{
}


void Eye::updateMatrix() const
{
	mMatrix.setToIdentity();
	mMatrix.rotate( -rotation() );
	mMatrix.translate( -position() );
}


void Eye::updateSelf( const float & delta )
{
	ALfloat listenerOri[]={(ALfloat)-direction().x(),(ALfloat)-direction().y(),(ALfloat)-direction().z(),
		(ALfloat)up().x(),(ALfloat)up().y(),(ALfloat)up().z()};
	alListenerfv( AL_POSITION, reinterpret_cast<const ALfloat*>(&position()) );
	alListenerfv( AL_ORIENTATION, listenerOri );
//	ALfloat listenerVel[]={0.0,0.0,0.0};
//	alListenerfv( AL_VELOCITY, listenerVel);
}


void Eye::drawSelf()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( mFOV, (float)scene()->width()/scene()->height(), mNearPlane, mFarPlane );
	glMatrixMode( GL_MODELVIEW );

	QMap<int,QVector4D>::const_iterator i = mClippingPlanes.constBegin();
	while( i != mClippingPlanes.constEnd() )
	{
		double plane[4]={ i.value().x(), i.value().y(), i.value().z(), i.value().w() };
		glClipPlane( GL_CLIP_PLANE0 + i.key(), plane );
		glEnable( GL_CLIP_PLANE0 + i.key() );
		++i;
	}
}


void Eye::drawSelfPost()
{
	QMap<int,QVector4D>::const_iterator i = mClippingPlanes.constBegin();
	while( i != mClippingPlanes.constEnd() )
	{
		glDisable( GL_CLIP_PLANE0 + i.key() );
		++i;
	}
}


void Eye::setClippingPlane( int n, QVector4D plane )
{
	if( plane.isNull() )
	{
		mClippingPlanes.remove( n );
	} else {
		mClippingPlanes[n] = plane;
	}
}
