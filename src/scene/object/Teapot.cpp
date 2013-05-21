#include "Teapot.hpp"
#include "../Scene.hpp"
#include "geometry/teapot.h"

#include <QString>

#include <stdio.h>

#include <resource/Material.hpp>
#include <resource/AudioSample.hpp>


Teapot::Teapot( Scene * scene, const float & size ) :
	AObject( scene, size ),
	mSize( size )
{
	mMaterial = new Material( scene->glWidget(), "KirksEntry" );
	mAudioSample = new AudioSample( "data/sound/test.ogg" );
	mAudioSample->setLooping( true );
	mAudioSample->setRolloffFactor( 10.0f );
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
	teapot( 8, mSize, GL_FILL );
	mMaterial->release();
}
