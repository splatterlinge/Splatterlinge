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

	QSharedPointer<AWeapon> weapon() { return *mCurrentWeapon; }

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
	bool mReloadPressed;
	bool mSpeedPressed;
	bool mGodMode;
	bool mOnGround;
	float mVelocityY;
	float mHeightAboveGround;
	float mAxisRotationX;
	float mAxisRotationY;
	bool mDragTeapot;
	bool mDragTorch;
	QVector3D mGroundNormal;
	QSharedPointer<Torch> mTorch;
	QList< QSharedPointer<AWeapon> > mWeapons;
	QList< QSharedPointer<AWeapon> >::iterator mCurrentWeapon;
};


#endif
