#include "Teapot.hpp"

#include <QString>

#include "GLScene.hpp"
#include "Material.hpp"
#include "teapot.h"


Teapot::Teapot( GLScene * scene, const Object3D * parent ) :
	Object3D( scene, parent )
{
	mMaterial = new Material( scene->glWidget(), "KirksEntry" );
}


Teapot::~Teapot()
{
	delete mMaterial;
}


void Teapot::updateSelf( const float & delta )
{
}


void Teapot::drawSelf()
{
	mMaterial->bind();
	glPushMatrix();
	glTranslatef( 0, -16, 0 );
	glDisable( GL_CULL_FACE );
	teapot( 20, 16.0f, GL_FILL );
	glEnable( GL_CULL_FACE );
	glPopMatrix();
	mMaterial->release();
}
