#include "Teapot.hpp"

#include <QString>

#include "GLScene.hpp"
#include "Material.hpp"
#include "teapot.h"


Teapot::Teapot( GLScene * scene ) :
	Object3D( scene )
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
	teapot( 8, 16.0f, GL_FILL );
	glEnable( GL_CULL_FACE );
	glPopMatrix();
	mMaterial->release();
}
