#include "Object3D.hpp"

#include <GL/gl.h>


Object3D::Object3D( GLScene * scene ) :
	mScene(scene),
	mParent(),
	mPosition(0,0,0),
	mRotation(),
	mSubNodes(),
	mMatrixNeedsUpdate(true)
{
}


Object3D::Object3D( const Object3D & other ) :
	mMatrix(other.mMatrix),
	mScene(other.mScene),
	mParent(other.mParent),
	mPosition(other.mPosition),
	mRotation(other.mRotation),
	mSubNodes(other.mSubNodes),
	mMatrixNeedsUpdate(other.mMatrixNeedsUpdate)
{
}


Object3D::~Object3D()
{
	mSubNodes.clear();
}


Object3D & Object3D::operator=( const Object3D & other )
{
	mMatrix = other.mMatrix;
	mScene =other.mScene;
	mParent = other.mParent;
	mPosition = other.mPosition;
	mRotation = other.mRotation;
	mSubNodes = other.mSubNodes;
	mMatrixNeedsUpdate = other.mMatrixNeedsUpdate;
	return *this;
}


void Object3D::updateMatrix() const
{
	mMatrix.setToIdentity();
	mMatrix.rotate( -rotation() );
	mMatrix.translate( -position() );
}


void Object3D::update( const float & delta )
{
	updateSelf( delta );
	QList< QSharedPointer<Object3D> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->update( delta );
	}
	updateSelfPost( delta );
}


void Object3D::draw()
{
	glPushMatrix();
	glMultMatrixd( matrix().constData() );
	drawSelf();
	QList< QSharedPointer<Object3D> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->draw();
	}
	drawSelfPost();
	glPopMatrix();
}


void Object3D::drawPostProc()
{
	glPushMatrix();
	glMultMatrixd( matrix().constData() );
	drawSelfPostProc();
	QList< QSharedPointer<Object3D> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->drawPostProc();
	}
	glPopMatrix();
}


void Object3D::add( QSharedPointer<Object3D> other )
{
	if( other->parent() )
		other->parent()->remove( other );
	mSubNodes.append( other );
	other->setParent( this );
}


void Object3D::remove( QSharedPointer<Object3D> other )
{
	other->setParent( 0 );
	mSubNodes.removeOne( other );
}
