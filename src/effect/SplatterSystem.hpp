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
	SplatterSystem( GLWidget * glWidget, Terrain * terrain, Material * splatterMaterial, Material * particleMaterial,
			int maxSplatters = 200, int maxParticles = 500 );
	virtual ~SplatterSystem();
	void update( const double & delta );
	void draw( const QMatrix4x4 & modelView );
	void spray( const QVector3D & source, float size );
	void splat( const QVector3D & source, float size );

	Material * splatterMaterial() { return mSplatterMaterial; }
	void setSplatterMaterial( Material * splatterMaterial ) { mSplatterMaterial = splatterMaterial; }

	Material * particleMaterialMaterial() { return mParticleMaterial; }
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
