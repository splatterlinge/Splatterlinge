/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

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
	mAspect( 1.0f ),
	mNearPlane( 0.1f ),
	mFarPlane( 500.0f ),
	mViewOffset(),
	mPerspectiveOffset()
{
}


Eye::Eye( Eye & other ) :
	mScene( other.mScene ),
	mPosition( other.mPosition ),
	mRotation( other.mRotation ),
	mScale( other.mScale ),
	mFOV( other.mFOV ),
	mAspect( other.mAspect ),
	mNearPlane( other.mNearPlane ),
	mFarPlane( other.mFarPlane ),
	mViewOffset( other.mViewOffset ),
	mPerspectiveOffset( other.mPerspectiveOffset )
{
}


Eye::~Eye()
{
}


void Eye::update( const double & delta )
{
	if( !mAttached.isNull() )
	{
		mPosition = mAttached.data()->position();
		mRotation = mAttached.data()->rotation();
	}
	mVelocity = (mPosition - mLastPosition) / delta;
	mLastPosition = mPosition;
}


void Eye::applyAL()
{
	QVector3D up = mRotation.rotatedVector( QVector3D(0,1,0) );
	QVector3D direction = mRotation.rotatedVector( QVector3D(0,0,1) );
	ALfloat listenerOri[] = {
		(ALfloat)direction.x(), (ALfloat)direction.y(), (ALfloat)direction.z(),
		(ALfloat)up.x(), (ALfloat)up.y(), (ALfloat)up.z()
	};
	alListener( AL_POSITION, mPosition );
	alListenerv( AL_ORIENTATION, listenerOri );
	alListener( AL_VELOCITY, mVelocity );
}


void Eye::applyGL()
{
	mProjectionMatrix.setToIdentity();
	mProjectionMatrix.translate( -mPerspectiveOffset );
	mProjectionMatrix.perspective( mFOV, mAspect, mNearPlane, mFarPlane );

	mViewMatrixInverse.setToIdentity();
	mViewMatrixInverse.translate( mPosition );
	mViewMatrixInverse.rotate( mRotation );
	mViewMatrixInverse.rotate( 180.0f, QVector3D( 0, 1, 0 ) );
	mViewMatrixInverse.translate( mViewOffset );
	mViewMatrixInverse.scale( mScale );
	mViewMatrix = mViewMatrixInverse.inverted();

	glMatrixMode( GL_PROJECTION );
	glLoadMatrix( mProjectionMatrix );

	glMatrixMode( GL_MODELVIEW );
	glLoadMatrix( mViewMatrix );

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
