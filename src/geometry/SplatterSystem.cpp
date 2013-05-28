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
	mParticleSystem->setDrag( 0.9f );
	mSplatterMaterial = new Material( glWidget, "SplatterBig" );
	mParticleMaterial = new Material( glWidget, "Splatter" );
}


SplatterSystem::~SplatterSystem()
{
	delete mParticleSystem;
}


void SplatterSystem::update( const double & delta )
{
	mParticleSystem->update( delta );
	for( int i = 0; i < mSplatters.size(); ++i )
	{
		mSplatters[i].fade -= delta*0.1f;
	}
}


void SplatterSystem::draw()
{
	mParticleMaterial->bind();
	mParticleSystem->draw();
	mParticleMaterial->release();

	mSplatterMaterial->bind();
	glEnable( GL_BLEND );
	glBlendFunc( GL_DST_COLOR, GL_ZERO );
	foreach( Splatter s, mSplatters )
	{
		if( s.fade <= 0.0f )
			continue;
		glMatrixMode( GL_TEXTURE );
		glActiveTexture( GL_TEXTURE0 );
		glPushMatrix();
		QRect mapRect = mTerrain->toMap( s.rect );
		glScale( 1.0/mapRect.size().width(), 1.0/mapRect.size().height(), 1.0 );
		glTranslated( -mapRect.x(), -mapRect.y(), 0.0 );

		QVector4D c = interpolateLinear( QVector4D(1.0f,1.0f,1.0f,1.0f), mSplatterMaterial->constData()->emission(), s.fade );
		mSplatterMaterial->overrideEmission( c );

		mTerrain->drawPatchMap( mapRect );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW);
	}
	glDisable( GL_BLEND );
	mSplatterMaterial->release();
}


void SplatterSystem::spray( const QVector3D & source, const float & size )
{
	mParticleSystem->emitSpherical( source, 30.0f, 30.0f, 50.0f );
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
