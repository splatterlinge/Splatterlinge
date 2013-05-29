#ifndef GEOMETRY_SPLATTERSYSTEM_INCLUDED
#define GEOMETRY_SPLATTERSYSTEM_INCLUDED

#include <GLWidget.hpp>

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include "Terrain.hpp"


class AudioSample;
class Material;
class ParticleSystem;


class SplatterSystem
{
public:
	SplatterSystem( GLWidget * glWidget, Terrain * terrain, int maxSplatters = 50, int maxParticles = 500 );
	~SplatterSystem();
	void update( const double & delta );
	void draw();
	void spray( const QVector3D & source, const float & size );

	float splatterFadeSpeed() { return mSplatterFadeSpeed; }
	void setSplatterFadeSpeed( float speed ) { mSplatterFadeSpeed = speed; }

protected:

private:
	class Splatter
	{
	public:
		Splatter( const QRectF & _rect = QRectF(0,0,0,0) ) : rect(_rect), fade(0.0f) {}
		QRectF rect;
		float fade;
	};
	GLWidget * mGLWidget;
	Terrain * mTerrain;
	QVector< Splatter > mSplatters;
	ParticleSystem * mParticleSystem;
	Material * mSplatterMaterial;
	Material * mParticleMaterial;
	float mSplatterFadeSpeed;
	QVector< AudioSample * > mBurstSampleSources;
};


#endif
