#ifndef SCENE_OBJECT_WORLD_INCLUDED
#define SCENE_OBJECT_WORLD_INCLUDED


#include <scene/AKeyListener.hpp>
#include <scene/AMouseListener.hpp>
#include <geometry/ParticleSystem.hpp>

#include "AObject.hpp"
#include "Player.hpp"
#include "Sky.hpp"
#include "Landscape.hpp"
#include "WavefrontObject.hpp"


class Shader;
class TextureRenderer;
class Material;
class SplatterSystem;


/// World object
/**
 *
 */
class World : public AObject, public AKeyListener
{
public:
	World( Scene * scene, QString name );
	virtual ~World();

	virtual void updateSelf( const double & delta );
	virtual void updateSelfPost( const double & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();

	virtual void keyPressEvent( QKeyEvent * event );
	virtual void keyReleaseEvent( QKeyEvent * event );

	void addLightSource( ALightSource * lightSource );
	void removeLightSource( ALightSource * lightSource );

	SplatterSystem * splatterSystem() { return mSplatterSystem; }

	QSharedPointer<Landscape> landscape() { return mLandscape; }
	QSharedPointer<Sky> sky() { return mSky; }

	QSharedPointer<AObject> getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & length, QVector3D & normal );

private:
	class SplatterInteractor : public ParticleSystem::Interactable
	{
	public:
		SplatterInteractor( World & world ) : mWorld(world) {}
		virtual ~SplatterInteractor() {}
		virtual void particleInteraction( const double & delta, ParticleSystem::Particle & particle );
	private:
		World & mWorld;
	};
	SplatterInteractor * mSplatterInteractor;
	bool mTimeLapse;
	bool mTimeReverse;
	float mTimeOfDay;
	QSharedPointer<Sky> mSky;
	QSharedPointer<Landscape> mLandscape;
	QSharedPointer<Player> mPlayer;
	QSharedPointer<WavefrontObject> mTable;
	QSharedPointer<WavefrontObject> mTree;
	QVector3D mTarget;
	QVector3D mTargetNormal;
	SplatterSystem * mSplatterSystem;
	QList< ALightSource * > mLightSources;
};


#endif
