#include "AObject.hpp"

#include <scene/Scene.hpp>
#include <GLWidget.hpp>

#include <float.h>


AObject::AObject( Scene * scene, float boundingSphereRadius ) :
	mScene(scene),
	mParent(),
	mPosition(0,0,0),
	mRotation(),
	mBoundingSphereRadius(boundingSphereRadius),
	mSubNodes(),
	mMatrixNeedsUpdate(true)
{
}


AObject::AObject( const AObject & other ) :
	mMatrix(other.mMatrix),
	mScene(other.mScene),
	mParent(other.mParent),
	mPosition(other.mPosition),
	mRotation(other.mRotation),
	mBoundingSphereRadius(other.mBoundingSphereRadius),
	mSubNodes(other.mSubNodes),
	mMatrixNeedsUpdate(other.mMatrixNeedsUpdate)
{
}


AObject::~AObject()
{
	mSubNodes.clear();
}


AObject & AObject::operator=( const AObject & other )
{
	mMatrix = other.mMatrix;
	mScene =other.mScene;
	mParent = other.mParent;
	mPosition = other.mPosition;
	mRotation = other.mRotation;
	mBoundingSphereRadius = other.mBoundingSphereRadius;
	mSubNodes = other.mSubNodes;
	mMatrixNeedsUpdate = other.mMatrixNeedsUpdate;
	return *this;
}


void AObject::updateMatrix() const
{
	mMatrix.setToIdentity();
	mMatrix.rotate( rotation() );
	mMatrix.translate( position() );
}


void AObject::update( const float & delta )
{
	updateSelf( delta );
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->update( delta );
	}
	updateSelfPost( delta );
}


void AObject::draw()
{
	glPushMatrix();
#if QT_VERSION < 0x050000
	glMultMatrixd( matrix().constData() );
#else
	glMultMatrixf( matrix().constData() );
#endif
	drawSelf();
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		if( (*i)->boundingSphereRadius() > FLT_EPSILON )	// nonzero radius -> do frustum culling
		{
			if( scene()->eye()->isSphereInFrustum( (*i)->position(), (*i)->boundingSphereRadius() ) )
			{
				(*i)->draw();
			}
		} else {	// zero radius -> no frustum culling
			(*i)->draw();
		}
	}
	drawSelfPost();
	if( sDebugBoundingSpheres )
		drawBoundingShpere();
	glPopMatrix();
}


void AObject::drawPostProc()
{
	glPushMatrix();
#if QT_VERSION < 0x050000
	glMultMatrixd( matrix().constData() );
#else
	glMultMatrixf( matrix().constData() );
#endif
	drawSelfPostProc();
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		if( (*i)->boundingSphereRadius() > FLT_EPSILON )	// nonzero radius -> do frustum culling
		{
			if( scene()->eye()->isSphereInFrustum( (*i)->position(), (*i)->boundingSphereRadius() ) )
			{
				(*i)->drawPostProc();
			}
		} else {	// zero radius -> no frustum culling
			(*i)->drawPostProc();
		}
	}
	glPopMatrix();
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
	glPushAttrib( GL_POLYGON_BIT );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glColor3f( 1, 1, 1 );
	gluSphere( q, mBoundingSphereRadius, 16, 16 );
	glPopAttrib();
	gluDeleteQuadric( q );
}


bool AObject::sDebugBoundingSpheres = false;
