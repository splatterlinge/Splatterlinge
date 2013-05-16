#include "AObject.hpp"

#include <GLWidget.hpp>


AObject::AObject( Scene * scene ) :
	mScene(scene),
	mParent(),
	mPosition(0,0,0),
	mRotation(),
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
#if QT_VERSION < 0x05
    glMultMatrixd( matrix().constData() );
#else
    glMultMatrixf( matrix().constData() );
#endif
	drawSelf();
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->draw();
	}
	drawSelfPost();
	glPopMatrix();
}


void AObject::drawPostProc()
{
	glPushMatrix();
#if QT_VERSION < 0x05
    glMultMatrixd( matrix().constData() );
#else
    glMultMatrixf( matrix().constData() );
#endif
	drawSelfPostProc();
	QList< QSharedPointer<AObject> >::iterator i;
	for( i = mSubNodes.begin(); i != mSubNodes.end(); ++i )
	{
		(*i)->drawPostProc();
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
