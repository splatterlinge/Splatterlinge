#ifndef SCENE_OBJECT_WORLD_INCLUDED
#define SCENE_OBJECT_WORLD_INCLUDED


#include "../AKeyListener.hpp"
#include "../AMouseListener.hpp"
#include "AObject.hpp"


class Player;
class Sky;
class Landscape;
class Teapot;
class WavefrontObject;
class Shader;
class TextureRenderer;
class Material;
class SplatterSystem;


/// World object
/**
 *
 */
class World : public AObject, public AKeyListener, public AMouseListener
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
	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent( MouseMoveEvent * event );
	virtual void mouseWheelEvent( QGraphicsSceneWheelEvent * event );

	SplatterSystem * splatterSystem() { return mSplatterSystem; }

	QSharedPointer<Landscape> landscape() { return mLandscape; }
	QSharedPointer<Sky> sky() { return mSky; }

	QSharedPointer<AObject> getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & length, QVector3D & normal );

private:
	bool mTimeLapse;
	bool mTimeReverse;
	float mTimeOfDay;
	QSharedPointer<Sky> mSky;
	QSharedPointer<Landscape> mLandscape;
	QSharedPointer<Player> mPlayer;
	QSharedPointer<Teapot> mTeapot;
	QSharedPointer<WavefrontObject> mTable;
	QSharedPointer<WavefrontObject> mTree;
	QVector3D mTarget;
	QVector3D mTargetNormal;
	bool mDragTeapot;
	SplatterSystem * mSplatterSystem;
};


#endif
