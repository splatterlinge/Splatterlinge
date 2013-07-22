#ifndef GEOMETRY_SPLATTERSYSTEM_INCLUDED
#define GEOMETRY_SPLATTERSYSTEM_INCLUDED

#include <GLWidget.hpp>

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include "ParticleSystem.hpp"
#include "Terrain.hpp"


class AudioSample;
class Material;


/// Simulates splatter on a terrain
class SplatterSystem : ParticleSystem::Interactable
{
public:
	SplatterSystem( GLWidget * glWidget, Terrain * terrain, int maxSplatters = 200, int maxParticles = 500 );
	virtual ~SplatterSystem();
	void update( const double & delta );
	void draw( const QMatrix4x4 & modelView );
	void spray( const QVector3D & source, float size );
	void splat( const QVector3D & source, float size );

	float splatterFadeSpeed() const { return mSplatterFadeSpeed; }
	void setSplatterFadeSpeed( float speed ) { mSplatterFadeSpeed = speed; }

	float splatterDriftFactor() const { return mSplatterDriftFactor; }
	void setSplatterDriftFactor( float driftFactor ) { mSplatterDriftFactor = driftFactor; }

	float burstPitchRange() const { return mBurstPitchRange; }
	void setBurstPitchRange( float range ) { mBurstPitchRange = range; }

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
	QVector< AudioSample * > mBurstSampleSources;
};


#endif
