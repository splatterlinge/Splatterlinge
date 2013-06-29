#include "SplatterSystem.hpp"

#include "ParticleSystem.hpp"

#include <GLWidget.hpp>
#include <utility/interpolate.hpp>
#include <utility/random.hpp>
#include <resource/Material.hpp>
#include <resource/AudioSample.hpp>

#include <math.h>
#include <float.h>


SplatterSystem::SplatterSystem( GLWidget * glWidget, Terrain * terrain, int maxSplatters, int maxParticles ) :
	mGLWidget( glWidget ),
	mTerrain( terrain ),
	mSplatters( maxSplatters )
{
	mParticleSystem = new ParticleSystem( maxParticles );
	mParticleSystem->setSize( 4.0f );
	mParticleSystem->setGravity( QVector3D( 0.0f, -120.0f, 0.0f ) );
	mParticleSystem->setDrag( 0.25f );
	mParticleSystem->setInteractionCallback( this );

	mSplatterFadeSpeed = 0.3f;
	mSplatterDriftFactor = 100.0f;

	mBurstPitchRange = 0.3;

	mSplatterMaterial = new Material( glWidget, "SplatterBig" );
	mParticleMaterial = new Material( glWidget, "Splatter" );

	mBurstSampleSources.resize( 4 );
	for( int i=0; i<mBurstSampleSources.size(); ++i )
	{
		mBurstSampleSources[i] = new AudioSample( "./data/sound/splatter.ogg" );
		mBurstSampleSources[i]->setLooping( false );
		mBurstSampleSources[i]->setRolloffFactor( 0.05f );
	}
}


SplatterSystem::~SplatterSystem()
{
	delete mParticleSystem;
	delete mParticleMaterial;
	delete mSplatterMaterial;
	for( int i=0; i<mBurstSampleSources.size(); ++i )
	{
		delete mBurstSampleSources[i];
	}
}


void SplatterSystem::update( const double & delta )
{
	mParticleSystem->update( delta );
	for( int i = 0; i < mSplatters.size(); ++i )
	{
		mSplatters[i].fade -= mSplatterFadeSpeed * delta;
	}
}


void SplatterSystem::draw()
{
	mParticleMaterial->bind();
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	mParticleSystem->draw();
	mParticleMaterial->release();

	mSplatterMaterial->bind();
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glEnable( GL_BLEND );
	glBlendFunc( GL_DST_COLOR, GL_ZERO );
	glMatrixMode( GL_TEXTURE );
	glActiveTexture( GL_TEXTURE0 );
	for( int i = 0; i < mSplatters.size(); ++i )
	{
		if( mSplatters[i].fade <= 0.0f )
			continue;

		QVector4D c = interpolateLinear( QVector4D(1.0f,1.0f,1.0f,1.0f), mSplatterMaterial->constData()->emission(), sqrtf(mSplatters[i].fade) );
		mSplatterMaterial->overrideEmission( c );

		glPushMatrix();
		QRectF mapRect = mTerrain->toMapF( mSplatters[i].rect );

		// rotate around center of texture in 90 deg. steps
		glTranslate( 0.5f, 0.5f, 0.0f );
		glRotate( mSplatters[i].rotation*90.0f, 0.0f, 0.0f, 1.0f );
		glTranslate( -0.5f, -0.5f, 0.0f );

		// transform texture coordinates to terrain patch
		float sizeFactor = powf( mSplatters[i].fade, mSplatterDriftFactor );
		QSizeF border = ( sizeFactor * mapRect.size() ) / 2.0f;
		glScale( 1.0/(mapRect.size().width()*(1.0f-sizeFactor)), 1.0/(mapRect.size().height()*(1.0f-sizeFactor)), 1.0 );
		glTranslate( -mapRect.x()-border.width(), -mapRect.y()-border.height(), 0.0 );

		float fX( mapRect.x()-(int)mapRect.x() );
		float fY( mapRect.y()-(int)mapRect.y() );
		QRect drawRect
		(
			floorf(mapRect.x()), floorf(mapRect.y()),
			ceilf(mapRect.width()+fX), ceilf(mapRect.height()+fY)
		);
		mTerrain->drawPatchMap( drawRect );
		glPopMatrix();
	}
	glMatrixMode( GL_MODELVIEW);
	glDisable( GL_BLEND );
	mSplatterMaterial->release();
}


void SplatterSystem::spray( const QVector3D & source, float size )
{
	mParticleSystem->emitSpherical( source, 0.05f*size*size, 1.0f*size, 1.75f*size );

	int maxSecOffset = 0;
	for( int i=0; i<mBurstSampleSources.size(); ++i )
	{
		if( !mBurstSampleSources[i]->isPlaying() )
		{
			maxSecOffset = i;
			break;
		}
		if( mBurstSampleSources[i]->secOffset() > mBurstSampleSources[maxSecOffset]->secOffset() )
		{
			maxSecOffset = i;
		}
	}
	mBurstSampleSources[maxSecOffset]->setPosition( source );
	mBurstSampleSources[maxSecOffset]->rewind();
	mBurstSampleSources[maxSecOffset]->setPitch( randomMinMax( 1.0f-mBurstPitchRange*0.5, 1.0+mBurstPitchRange*0.5 ) );
	mBurstSampleSources[maxSecOffset]->play();
}


void SplatterSystem::splat( const QVector3D & source, float size )
{
	int minFadeSplatter = 0;
	for( int i = 0; i < mSplatters.size(); ++i )
	{
		if( mSplatters[i].fade <= 0.0f )
		{
			minFadeSplatter = i;
			break;
		}
		if( mSplatters[i].fade < mSplatters[minFadeSplatter].fade )
		{
			minFadeSplatter = i;
		}
	}
	mSplatters[minFadeSplatter].fade = 1.0f;
	mSplatters[minFadeSplatter].rect = QRectF( source.x()-size*0.5f, source.z()-size*0.5f, size, size );
}


void SplatterSystem::particleInteraction( const double & delta, ParticleSystem::Particle & particle )
{
	if( mTerrain->getHeightAboveGround( particle.position() ) > -particleSystem()->size()/2.0f )
		return;
	particle.setLife( 0.0f );

	splat( particle.position(), particleSystem()->size() * randomMinMax( 0.5f, 2.0f ) );
}
