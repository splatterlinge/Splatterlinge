#ifndef SCENE_OBJECT_PLAYER_INCLUDED
#define SCENE_OBJECT_PLAYER_INCLUDED


#include "ACreature.hpp"
#include "../Teapot.hpp"
#include "../Torch.hpp"
#include "../weapon/AWeapon.hpp"
#include <scene/AKeyListener.hpp>
#include <scene/AMouseListener.hpp>

#include <GLWidget.hpp>


class Player : public ACreature, public AKeyListener, public AMouseListener
{
public:
	Player( World * world );
	virtual ~Player();

	virtual void updateSelf( const double & delta );
	virtual void update2Self( const double & delta );
	virtual void drawSelf();
	virtual void draw2Self();

	virtual void keyPressEvent( QKeyEvent * event );
	virtual void keyReleaseEvent( QKeyEvent * event );

	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent( MouseMoveEvent * event );
	virtual void mouseWheelEvent( QGraphicsSceneWheelEvent * event );

private:
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
	bool mDragTeapot;
	bool mDragTorch;
	QSharedPointer<Torch> mTorch;
	QList< QSharedPointer<AWeapon> > mWeapons;
	QSharedPointer<AWeapon> mCurrentWeapon;
};

#endif
