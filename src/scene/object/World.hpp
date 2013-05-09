#ifndef SCENE_OBJECT_WORLD_INCLUDED
#define SCENE_OBJECT_WORLD_INCLUDED


#include "../KeyListener.hpp"
#include "../MouseListener.hpp"
#include "AObject.hpp"


class Sky;
class Landscape;
class Teapot;
class Shader;
class TextureRenderer;


class World : public AObject, public KeyListener, public MouseListener
{
public:
	World( Scene * scene );
	virtual ~World();

	virtual void updateSelf( const float & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();

	virtual void keyPressEvent( QKeyEvent * event );
	virtual void keyReleaseEvent( QKeyEvent * event );
	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	virtual void wheelEvent( QGraphicsSceneWheelEvent * event );

	QSharedPointer<AObject> getLineIntersection( const QVector3D & origin, const QVector3D & direction, float * length );

private:
	bool mTimeLapse;
	float mTimeOfDay;
	Sky * mSky;
	QSharedPointer<Landscape> mLandscape;
	QSharedPointer<Teapot> mTeapot;
	bool mDragTeapot;
};


#endif
