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

#include "AObject.hpp"

#include <scene/object/Eye.hpp>
#include <scene/Scene.hpp>
#include <GLWidget.hpp>

#include <float.h>


AObject::AObject( Scene * scene, float boundingSphereRadius ) :
	mScene( scene ),
	mParent(),
	mPosition( 0, 0, 0 ),
	mRotation(),
	mBoundingSphereRadius( boundingSphereRadius ),
	mSubNodes(),
	mModelMatrix(),
	mModelMatrixNeedsUpdate( true )
{
}


AObject::AObject( const AObject & other ) :
	mScene( other.mScene ),
	mParent( other.mParent ),
	mPosition( other.mPosition ),
	mRotation( other.mRotation ),
	mBoundingSphereRadius( other.mBoundingSphereRadius ),
	mSubNodes( other.mSubNodes ),
	mModelMatrix( other.mModelMatrix ),
	mModelMatrixNeedsUpdate( other.mModelMatrixNeedsUpdate )
{
}


AObject::~AObject()
{
	mSubNodes.clear();
}


AObject & AObject::operator=( const AObject & other )
{
	mModelMatrix = other.mModelMatrix;
	mScene = other.mScene;
	mParent = other.mParent;
	mPosition = other.mPosition;
	mRotation = other.mRotation;
	mBoundingSphereRadius = other.mBoundingSphereRadius;
	mSubNodes = other.mSubNodes;
	mModelMatrixNeedsUpdate = other.mModelMatrixNeedsUpdate;
	return *this;
}


void AObject::syncMatrix() const
{
	if( mParent )
	{
		mModelMatrix = mParent->modelMatrix();
	} else {
		mModelMatrix.setToIdentity();
	}
	mModelMatrix.translate( position() );
	mModelMatrix.rotate( rotation() );
}


void AObject::update( const double & delta )
{
	syncMatrix();
	updateSelf( delta );
	QLinkedList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->update( delta );
	}
	updateSelfPost( delta );
}


void AObject::update2( const double & delta )
{
	update2Self( delta );
	QLinkedList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->update2( delta );
	}
	update2SelfPost( delta );
}


void AObject::draw()
{
	mModelViewMatrix = scene()->eye()->viewMatrix() * modelMatrix();
	if( mSubNodes.size() )
		mFrustumTest.sync( mScene->eye()->projectionMatrix(), mModelViewMatrix );

	glLoadMatrix( mModelViewMatrix );
	drawSelf();

	QLinkedList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		if( (*i)->boundingSphereRadius() > FLT_EPSILON )	// nonzero radius -> do frustum culling
		{
			if( mFrustumTest.isSphereInFrustum( (*i)->position(), (*i)->boundingSphereRadius() ) )
			{
				(*i)->draw();
			}
		} else {	// zero radius -> no frustum culling
			(*i)->draw();
		}
	}

	glLoadMatrix( mModelViewMatrix );
	drawSelfPost();

	if( sDebugBoundingSpheres )
		drawBoundingShpere();
}


void AObject::draw2()
{
	glLoadMatrix( mModelViewMatrix );
	draw2Self();

	QLinkedList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		if( (*i)->boundingSphereRadius() > FLT_EPSILON )	// nonzero radius -> do frustum culling
		{
			if( mFrustumTest.isSphereInFrustum( (*i)->position(), (*i)->boundingSphereRadius() ) )
			{
				(*i)->draw2();
			}
		} else {	// zero radius -> no frustum culling
			(*i)->draw2();
		}
	}

	glLoadMatrix( mModelViewMatrix );
	draw2SelfPost();
}


void AObject::add( QSharedPointer<AObject> other )
{
	if( other->parent() )
		other->parent()->remove( other );
	mSubNodes.append( other );
	other->setParent( this );
}


void AObject::remove( QSharedPointer<AObject> other )
{
	other->setParent( 0 );
	mSubNodes.removeOne( other );
}


void AObject::drawBoundingShpere()
{
	if( mBoundingSphereRadius <= FLT_EPSILON )
		return;
	GLUquadric * q = gluNewQuadric();
	glPushAttrib( GL_POLYGON_BIT | GL_LIGHTING_BIT | GL_ENABLE_BIT );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_LIGHTING );
	glColor3f( 1, 1, 1 );
	glDisable( GL_CULL_FACE );
	gluSphere( q, mBoundingSphereRadius, 16, 16 );
	glEnable( GL_CULL_FACE );
	glPopAttrib();
	gluDeleteQuadric( q );
}


const AObject * AObject::intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction,
	float & length, QVector3D * normal ) const
{
	const AObject * nearestTarget = NULL;
	QLinkedList< QSharedPointer<AObject> >::const_iterator i;
	for( i = mSubNodes.constBegin(); i != mSubNodes.constEnd(); ++i )
	{
		if( (*i).data() != exclude )
		{
			const AObject * target = (*i)->intersectLine( exclude, origin, direction, length, normal );
			if( target )
				nearestTarget = target;
		}
	}
	return nearestTarget;
}


QVector<const AObject*> AObject::collideSphere( const AObject * exclude, const float & radius,
	QVector3D & center, QVector3D * normal ) const
{
	QVector<const AObject*> collisions;
	QLinkedList< QSharedPointer<AObject> >::const_iterator i;
	for( i = mSubNodes.constBegin(); i != mSubNodes.constEnd(); ++i )
	{
		if( (*i).data() != exclude )
			collisions << ((*i)->collideSphere( exclude, radius, center, normal ));
	}
	return collisions;
}


bool AObject::sDebugBoundingSpheres = false;
