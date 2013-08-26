/*
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#include "SplatterSystem.hpp"

#include <GLWidget.hpp>
#include <geometry/Terrain.hpp>
#include <utility/Interpolation.hpp>
#include <utility/RandomNumber.hpp>
#include <resource/Material.hpp>
#include <resource/AudioSample.hpp>

#include <math.h>
#include <float.h>


SplatterSystem::SplatterSystem( GLWidget * glWidget, Terrain * terrain,
		const QString & splatterMaterialName, const QString & particleMaterialName,
		const QString & burstAudioSampleName,
		int maxSplatters, int maxParticles ) :
	mGLWidget( glWidget ),
	mTerrain( terrain ),
	mSplatters( maxSplatters )
{
	mSplatterMaterial = new Material( glWidget, splatterMaterialName );
	mParticleMaterial = new Material( glWidget, particleMaterialName );

	mParticleSystem = new ParticleSystem( maxParticles );
	mParticleSystem->setSize( 4.0f );
	mParticleSystem->setGravity( QVector3D( 0.0f, -120.0f, 0.0f ) );
	mParticleSystem->setDrag( 0.25f );
	mParticleSystem->setInteractionCallback( this );

	mSplatBelow = true;
	mSplatterFadeSpeed = 0.3f;
	mSplatterDriftFactor = 100.0f;

	mBurstPitchRange = 0.3;

	mBurstSampleSources.resize( 4 );
	for( int i=0; i<mBurstSampleSources.size(); ++i )
	{
		mBurstSampleSources[i] = new AudioSample( burstAudioSampleName );
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


void SplatterSystem::draw( const QMatrix4x4 & modelView )
{
	mParticleMaterial->bind();
	mParticleSystem->draw( modelView );
	mParticleMaterial->release();

	mSplatterMaterial->bind();
	glEnable( GL_BLEND );
	glBlendFunc( GL_DST_COLOR, GL_ZERO );
	glMatrixMode( GL_TEXTURE );
	for( int i = 0; i < mSplatters.size(); ++i )
	{
		if( mSplatters[i].fade <= 0.0f )
			continue;

		QVector4D c = Interpolation::linear( QVector4D(1.0f,1.0f,1.0f,1.0f), mSplatterMaterial->constData()->emission(), sqrtf(mSplatters[i].fade) );
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
	if( size < 10.0f ) size = 10.0f;
	if( size > 100.0f ) size = 100.0f;
	int numToEmit = 0.5f * size;
	if( numToEmit < 1 ) numToEmit = 1;
	mParticleSystem->emitSpherical( source, numToEmit, 0.25f*size, 1.0f*size );

	if( mSplatBelow && mTerrain->getHeightAboveGround( source ) < size*0.5f )
		splat( source, size * RandomNumber::minMax( 0.2f, 0.3f ) );

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
	mBurstSampleSources[maxSecOffset]->setPitch( RandomNumber::minMax( 1.0f-mBurstPitchRange*0.5, 1.0+mBurstPitchRange*0.5 ) );
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
}
