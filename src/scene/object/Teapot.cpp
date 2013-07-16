#include "Teapot.hpp"
#include "World.hpp"

#include <scene/Scene.hpp>
#include <geometry/teapot.h>
#include <resource/Material.hpp>
#include <resource/AudioSample.hpp>


Teapot::Teapot( Scene * scene, const float & size ) :
	AObject( scene, size*1.5f ),
	mSize( size )
{
	mMaterial = new Material( scene->glWidget(), "KirksEntry" );
	mAudioSample = new AudioSample( "data/sound/test.ogg" );
	mAudioSample->setLooping( true );
	mAudioSample->setRolloffFactor( 0.1f );
	mAudioSample->play();
}


Teapot::~Teapot()
{
	delete mMaterial;
	delete mAudioSample;
}


void Teapot::updateSelf( const double & delta )
{
	mAudioSample->setPosition( this->position() );
	World * world = dynamic_cast<World*>(scene()->root());
	if( world )
	{
		setRotation( QQuaternion::nlerp( rotation(), world->landscape()->terrain()->getNormalRotation(position()), 33.0*delta ) );
	}
}


void Teapot::drawSelf()
{
	mMaterial->bind();
	glPushMatrix();
	glTranslatef( 0, mSize*0.6, 0 );
	teapot( 6, mSize, GL_FILL );
	glPopMatrix();
	mMaterial->release();
}
