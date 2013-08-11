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

#ifndef SCENE_OBJECT_WORLD_INCLUDED
#define SCENE_OBJECT_WORLD_INCLUDED


#include <scene/AKeyListener.hpp>
#include <scene/AMouseListener.hpp>
#include <geometry/ParticleSystem.hpp>

#include "AObject.hpp"
#include "creature/Player.hpp"
#include "creature/Dummy.hpp"
#include "Sky.hpp"
#include "Landscape.hpp"
#include "environment/Forest.hpp"

#include <QTime>
#include <QVector>
#include <QMatrix4x4>

class Shader;
class TextureRenderer;
class Material;
class SplatterSystem;


/// Splatter quality settings
class SplatterQuality
{
	SplatterQuality() {}
	~SplatterQuality() {}
public:
	enum Type
	{
		LOW	= 0,
		MEDIUM	= 1,
		HIGH	= 2
	};
	const static int num = 3;

	static Type fromString( const QString & name );
	static QString toString( const Type & quality );
	static const Type & maximum() { return sMaximum; }
	static void setMaximum( const Type & max ) { sMaximum = max; }

private:
	static Type sMaximum;
};


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

	QSharedPointer<Player> player() { return mPlayer; }
	QSharedPointer<Teapot> teapot() { return mTeapot; }

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
	QSharedPointer<Teapot> mTeapot;
	QSharedPointer<Player> mPlayer;
	QSharedPointer<Dummy> mDummy;
	QSharedPointer<Forest> mForest;
	QVector3D mTarget;
	QVector3D mTargetNormal;
	SplatterSystem * mSplatterSystem;
	QList< ALightSource * > mLightSources;
};


#endif
