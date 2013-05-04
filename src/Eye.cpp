#include "Eye.hpp"


#include "GLWidget.hpp"
#include "GLScene.hpp"


Eye::Eye( GLScene * scene ) :
	Object3D( scene ),
	mFOV(60.0f),
	mNearPlane(0.1f),
	mFarPlane(200.0f)
{
}


Eye::Eye( Eye & other ) :
	Object3D(other),
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
