#include "Eye.hpp"


#include "GLWidget.hpp"
#include "GLScene.hpp"


Eye::Eye( GLScene * scene, const Object3D * parent ) :
	Object3D( scene, parent ),
	mFOV(60.0f),
	mNear(0.1f),
	mFar(400.0f)
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
	gluPerspective( mFOV, (float)scene()->width()/scene()->height(), mNear, mFar );
	glMatrixMode( GL_MODELVIEW );
}
