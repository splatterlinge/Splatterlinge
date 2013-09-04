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

#ifndef EFFECT_SPLATTERSYSTEM_INCLUDED
#define EFFECT_SPLATTERSYSTEM_INCLUDED

#include <GLWidget.hpp>
#include <geometry/ParticleSystem.hpp>

#include <QVector>
#include <QVector2D>
#include <QVector3D>


class AudioSample;
class Material;
class Terrain;


/// Simulates splatter on a terrain
class SplatterSystem : ParticleSystem::Interactable
{
public:
	SplatterSystem( GLWidget * glWidget, Terrain * terrain,
		const QString & splatterMaterialName, const QString & particleMaterialName,
		const QString & burstAudioSampleName,
		int maxSplatters = 200, int maxParticles = 500 );
	virtual ~SplatterSystem();
	void update( const double & delta );
	void draw( const QMatrix4x4 & modelView );
	void spray( const QVector3D & source, float size );
	void splat( const QVector3D & source, float size );

	Material * splatterMaterial() { return mSplatterMaterial; }
	void setSplatterMaterial( Material * splatterMaterial ) { mSplatterMaterial = splatterMaterial; }

	Material * particleMaterial() { return mParticleMaterial; }
	void setParticleMaterial( Material * particleMaterial ) { mParticleMaterial = particleMaterial; }

	const bool & splatBelow() const { return mSplatBelow; }
	void setSplatBelow( const bool & enable ) { mSplatBelow = enable; }

	const float & splatterFadeSpeed() const { return mSplatterFadeSpeed; }
	void setSplatterFadeSpeed( const float & speed ) { mSplatterFadeSpeed = speed; }

	const float & splatterDriftFactor() const { return mSplatterDriftFactor; }
	void setSplatterDriftFactor( const float & driftFactor ) { mSplatterDriftFactor = driftFactor; }

	const float & burstPitchRange() const { return mBurstPitchRange; }
	void setBurstPitchRange( const float & range ) { mBurstPitchRange = range; }

	ParticleSystem * particleSystem() const { return mParticleSystem; }

	// Overrides:
	virtual void particleInteraction( const double & delta, ParticleSystem::Particle & particle );

protected:

private:
	class Splatter
	{
	public:
		Splatter( const QRectF & _rect = QRectF(0,0,0,0) ) : rect(_rect), fade(0.0f), rotation(rand()%4) {}
		QRectF rect;
		float fade;
		int rotation;
	};
	GLWidget * mGLWidget;
	Terrain * mTerrain;
	QVector< Splatter > mSplatters;
	ParticleSystem * mParticleSystem;
	Material * mSplatterMaterial;
	Material * mParticleMaterial;
	float mSplatterFadeSpeed;
	float mSplatterDriftFactor;
	float mBurstPitchRange;
	bool mSplatBelow;
	QVector< AudioSample * > mBurstSampleSources;
};


#endif
