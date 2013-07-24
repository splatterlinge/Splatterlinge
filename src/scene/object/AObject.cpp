#include "AObject.hpp"

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
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->update( delta );
	}
	updateSelfPost( delta );
}


void AObject::update2( const double & delta )
{
	update2Self( delta );
	QList< QSharedPointer<AObject> >::iterator i;
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

	QList< QSharedPointer<AObject> >::iterator i;
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

	QList< QSharedPointer<AObject> >::iterator i;
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


AObject * AObject::intersectLine( const QVector3D & origin, const QVector3D & direction,
	float & length, QVector3D * normal )
{
	AObject * nearestTarget = NULL;
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		AObject * target = (*i)->intersectLine( origin, direction, length, normal );
		if( target )
			nearestTarget = target;
	}
	return nearestTarget;
}


bool AObject::collideSphere( const float & radius,
	QVector3D & center, QVector3D * normal )
{
	bool collision = false;
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		collision |= (*i)->collideSphere( radius, center, normal );
	}
	return collision;
}


bool AObject::sDebugBoundingSpheres = false;
