#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject( Scene * scene, QString filename, float scale ) :
	AObject( scene )
{
	mScene = scene;
	mFilename = filename;
	mScale = scale;

	mModel = new StaticModel( mScene->glWidget(), mFilename );

	setBoundingSphere( qMin( mModel->getSize().width(), mModel->getSize().height() )*mScale );
}

WavefrontObject::~WavefrontObject()
{
}

void WavefrontObject::updateSelf( const double & delta )
{
}

void WavefrontObject::drawSelf()
{
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_CULL_FACE );
	glEnable( GL_AUTO_NORMAL );
	glEnable( GL_NORMALIZE );
	glEnable( GL_LIGHTING );

	glPushMatrix();

	glScalef( 1.0*mScale, 1.0*mScale, 1.0*mScale );
	glColor3f( 1.0f, 1.0f, 1.0f );

	mModel->draw();

	glPopMatrix();
	glPopAttrib();
}
