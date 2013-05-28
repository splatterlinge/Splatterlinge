#include "SplatterSystem.hpp"

#include "ParticleSystem.hpp"

#include <GLWidget.hpp>
#include <utility/interpolate.hpp>
#include <utility/random.hpp>
#include <utility/qHashKeys.hpp>
#include <resource/Material.hpp>

#include <math.h>


SplatterSystem::SplatterSystem( GLWidget * glWidget, Terrain * terrain, int maxSplatters, int maxParticles ) :
	mGLWidget( glWidget ),
	mTerrain( terrain ),
	mSplatters( maxSplatters )
{
	mParticleSystem = new ParticleSystem( maxParticles );
	mParticleSystem->setSize( 4.0f );
	mParticleSystem->setMass( 0.05f );
	mParticleSystem->setDrag( 0.1f );
	
	mSplatterFadeSpeed = 0.3f;

	mSplatterMaterial = new Material( glWidget, "SplatterBig" );
	mParticleMaterial = new Material( glWidget, "Splatter" );
}


SplatterSystem::~SplatterSystem()
{
	delete mParticleSystem;
	delete mParticleMaterial;
	delete mSplatterMaterial;
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
	mParticleSystem->draw();
	mParticleMaterial->release();

	mSplatterMaterial->bind();
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glEnable( GL_BLEND );
	glBlendFunc( GL_DST_COLOR, GL_ZERO );
	glMatrixMode( GL_TEXTURE );
	glActiveTexture( GL_TEXTURE0 );
	foreach( Splatter s, mSplatters )
	{
		if( s.fade <= 0.0f )
			continue;

		QVector4D c = interpolateLinear( QVector4D(1.0f,1.0f,1.0f,1.0f), mSplatterMaterial->constData()->emission(), sqrtf(s.fade) );
		mSplatterMaterial->overrideEmission( c );

		glPushMatrix();
		QRectF mapRect = mTerrain->toMapF( s.rect );
		glScale( 1.0/mapRect.size().width(), 1.0/mapRect.size().height(), 1.0 );
		glTranslated( -mapRect.x(), -mapRect.y(), 0.0 );
		mTerrain->drawPatchMap( mTerrain->toMap( s.rect ) );
		glPopMatrix();
	}
	glMatrixMode( GL_MODELVIEW);
	glDisable( GL_BLEND );
	mSplatterMaterial->release();
}


void SplatterSystem::spray( const QVector3D & source, const float & size )
{
	mParticleSystem->emitSpherical( source, 2.0f*size, 1.5f*size, 2.0f*size );
	int minFadeSplatter = 0;
	for( int i = 0; i < mSplatters.size(); ++i )
	{
		if( mSplatters[i].fade < mSplatters[minFadeSplatter].fade )
		{
			minFadeSplatter = i;
			if( mSplatters[i].fade <= 0.0f )
				break;
		}
	}
	mSplatters[minFadeSplatter].fade = 1.0f;
	mSplatters[minFadeSplatter].rect = QRectF( source.x()-size*0.5f, source.z()-size*0.5f, size, size );
}
