#include "Teapot.hpp"
#include "../Scene.hpp"
#include "geometry/teapot.h"

#include <QString>

#include <stdio.h>

#include <resource/Material.hpp>
#include <resource/AudioSample.hpp>


Teapot::Teapot( Scene * scene ) :
	AObject( scene )
{
	mMaterial = new Material( scene->glWidget(), "KirksEntry" );
	mAudioSample = new AudioSample( "data/sound/test.ogg" );
	mAudioSample->setLooping( true );
	mAudioSample->setRolloffFactor( 40.0f );
	mAudioSample->setObjectHook( this );
	mAudioSample->play();
}


Teapot::~Teapot()
{
	delete mMaterial;
	delete mAudioSample;
}


void Teapot::updateSelf( const float & delta )
{
	mAudioSample->update();
}


void Teapot::drawSelf()
{
	mMaterial->bind();
	glDisable( GL_CULL_FACE );
	teapot( 8, 16.0f, GL_FILL );
	glEnable( GL_CULL_FACE );
	mMaterial->release();
}
