#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject( Scene * scene, QString filename, QVector<QMatrix4x4> * instances ) :
	AObject( scene ),
	mScene( scene ),
	mFilename( filename ),
	mInstances( instances)
{
	mModel = new StaticModel( mScene, mFilename );

	setBoundingSphere( qMax( mModel->getSize().width(), mModel->getSize().height() ) );
}


WavefrontObject::~WavefrontObject()
{
	delete mModel;
	mInstances->clear();
	delete mInstances;
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

	glColor3f( 1.0f, 1.0f, 1.0f );

	mModel->draw( mInstances );

	glPopMatrix();
	glPopAttrib();
}
