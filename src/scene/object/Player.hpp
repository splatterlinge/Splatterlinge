#ifndef SCENE_OBJECT_PLAYER_INCLUDED
#define SCENE_OBJECT_PLAYER_INCLUDED

#include "AObject.hpp"
#include <scene/AKeyListener.hpp>
#include <scene/AMouseListener.hpp>

#include <GLWidget.hpp>


class World;
class Scene;


class Player : public AObject, public AKeyListener, public AMouseListener
{
public:
	Player( Scene * scene, World * world );
	virtual ~Player();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void drawAfterSelf();

	virtual void keyPressEvent( QKeyEvent * event );
	virtual void keyReleaseEvent( QKeyEvent * event );

	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent( MouseMoveEvent * event );
	virtual void mouseWheelEvent( QGraphicsSceneWheelEvent * event );

private:
	World * mWorld;
	QVector3D mTarget;
	QVector3D mTargetNormal;
	QPointF mMouseDelta;
	bool mForwardPressed;
	bool mBackwardPressed;
	bool mLeftPressed;
	bool mRightPressed;
	bool mUpPressed;
	bool mDownPressed;
	bool mSpeedPressed;
	bool mGodMode;
	bool mOnGround;
	float mVelocityY;
	float mHeightAboveGround;
	float mAxisRotationX;
	float mAxisRotationY;
};

#endif
