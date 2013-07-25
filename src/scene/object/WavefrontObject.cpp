#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject( Scene * scene, QString filename, float scale ) :
    AObject( scene )
{
    mScene = scene;
    mFilename = filename;
    mScale = scale;

    load();
}

WavefrontObject::~WavefrontObject()
{
}

void WavefrontObject::load()
{
    if( !mObjects.contains( mFilename ) )
    {
        mObjects[mFilename] = new WavefrontModel( mScene->glWidget(), mFilename );
    }

    mModel = mObjects[mFilename];

    setBoundingSphere( qMax( mModel->getSize().width(), mModel->getSize().height() )/4 );
}

void WavefrontObject::updateSelf( const double & delta )
{
    // TODO
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

    mModel->render();

    glPopMatrix();
    glPopAttrib();
}
