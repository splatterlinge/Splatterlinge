#include "Teapot.hpp"
#include "../teapot.h"
#include "../Scene.hpp"

#include <QString>

#include <resources/Material.hpp>


Teapot::Teapot( Scene * scene ) :
	AObject( scene )
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
