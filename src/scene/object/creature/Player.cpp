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

#include "Player.hpp"


#include <scene/Scene.hpp>
#include <effect/SplatterSystem.hpp>
#include "../World.hpp"
#include "../Landscape.hpp"
#include "../weapon/Laser.hpp"
#include "../weapon/Minigun.hpp"


Player::Player( World * world ) :
	ACreature( world )
{
	setLife( 100 );
	setState( SPAWNING );
	mArmor = 25;

	mForwardPressed = false;
	mLeftPressed = false;
	mBackwardPressed = false;
	mRightPressed = false;
	mUpPressed = false;
	mDownPressed = false;
	mSpeedPressed = false;
	mGodMode = true;
	mOnGround = false;

	mVelocityY = 0.0f;
	mHeightAboveGround = 2.0f;
	mAxisRotationX = 0.0f;
	mAxisRotationY = 0.0f;

	mSpeedCounter = 0.0f;
	mSpeedCooldown = 0.0f;

	mTarget = QVector3D(0,0,0);
	mDragTeapot = false;

	mCurrentWeapon = QSharedPointer<AWeapon>();
	giveWeapon( QSharedPointer<AWeapon>( new Laser(world) ) );

	mTorch = QSharedPointer<Torch>( new Torch( world ) );
	mTorch->setPositionY( world->landscape()->terrain()->getHeight( QPointF(0,0) ) + 1.0f );
	world->add( mTorch );
	mDragTorch = false;

	scene()->addKeyListener( this );
	scene()->addMouseListener( this );
}


Player::~Player()
{
}


void Player::keyPressEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_W:
		mForwardPressed = true;
		break;
	case Qt::Key_A:
		mLeftPressed = true;
		break;
	case Qt::Key_S:
		mBackwardPressed = true;
		break;
	case Qt::Key_D:
		mRightPressed = true;
		break;
	case Qt::Key_R:
		currentWeapon()->reload();
		break;
	case Qt::Key_Space:
		mUpPressed = true;
		break;
	case Qt::Key_Shift:
		mSpeedPressed = true;
		break;
	case Qt::Key_Control:
		mDownPressed = true;
		break;
	default:
		return;
	}
	event->accept();
}


void Player::keyReleaseEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_W:
		mForwardPressed = false;
		break;
	case Qt::Key_A:
		mLeftPressed = false;
		break;
	case Qt::Key_S:
		mBackwardPressed = false;
		break;
	case Qt::Key_D:
		mRightPressed = false;
		break;
	case Qt::Key_Space:
		mUpPressed = false;
		break;
	case Qt::Key_Shift:
		mSpeedPressed = false;
		break;
	case Qt::Key_Control:
		mDownPressed = false;
		break;
	case Qt::Key_Tab:
		mGodMode = !mGodMode;
		break;
	default:
		return;
	}
	event->accept();
}


void Player::mouseMoveEvent( MouseMoveEvent * event )
{
	mMouseDelta += event->delta();
}


void Player::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		if( mCurrentWeapon )
			mCurrentWeapon->triggerPressed();
	}
	else if( event->button() == Qt::RightButton )
	{
		mDragTeapot = true;
	}
	else if( event->button() == Qt::MiddleButton )
	{
		mDragTorch = true;
	}

}


void Player::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		if( mCurrentWeapon )
			mCurrentWeapon->triggerReleased();
	}
	else if( event->button() == Qt::RightButton )
	{
		mDragTeapot = false;
	}
	else if( event->button() == Qt::MiddleButton )
	{
		mDragTorch = false;
	}
}


void Player::mouseWheelEvent( QGraphicsSceneWheelEvent * event )
{
	if( mWeapons.size() )
	{
		int currentWeaponindex = mWeapons.indexOf( mCurrentWeapon );
		if( currentWeaponindex < 0 )
		{
			if( mCurrentWeapon )
				mCurrentWeapon->holster();
			mCurrentWeapon = mWeapons.first();
			mCurrentWeapon->pull();
		}
		else if( event->delta() > 0 && currentWeaponindex < mWeapons.size()-1 )
		{
			if( mCurrentWeapon )
				mCurrentWeapon->holster();
			mCurrentWeapon = mWeapons.at( currentWeaponindex + 1 );
			mCurrentWeapon->pull();
		}
		else if( event->delta() < 0 && currentWeaponindex > 0 )
		{
			if( mCurrentWeapon )
				mCurrentWeapon->holster();
			mCurrentWeapon = mWeapons.at( currentWeaponindex - 1 );
			mCurrentWeapon->pull();
		}
	}
}


void Player::updateSelf( const double & delta )
{
	if( mGodMode )
	{
		setState( ALIVE );
		setLife( 100 );
	}

	switch( state() )
	{
		case SPAWNING:
			setState( ALIVE );
			break;
		case ALIVE:
			updateRotation( delta );
			updatePosition( delta );
			updateTarget( delta );

			if( mCurrentWeapon )
			{
				mCurrentWeapon->setPosition( QVector3D(-0.5f,-0.5f-0.1*QVector3D::dotProduct(QVector3D(0,1,0),direction()), 0.5f ) );
				if( mTargetAvailable )
					mCurrentWeapon->setTarget( &mTarget );
				else
					mCurrentWeapon->setTarget( NULL );
			}

			break;
		case DYING:
			setState( DEAD );
			break;
		case DEAD:
			break;
	}
}


void Player::update2Self( const double & delta )
{
	if( mTargetAvailable )
	{
		if( mDragTeapot )
		{
			world()->teapot()->setPosition( mTarget );
		}
		if( mDragTorch )
		{
			mTorch->setPosition( mTarget );
			mTorch->moveY( 1 );
		}
	}
}


void Player::drawSelf()
{

}


void Player::draw2Self()
{
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	glPushAttrib( GL_DEPTH_BUFFER_BIT );
	glDisable( GL_DEPTH_TEST );

	glPushMatrix();
	glLoadIdentity();

	switch( state() )
	{
		case SPAWNING:
			break;
		case ALIVE:
			drawCrosshair();
			break;
		case DYING:
			break;
		case DEAD:
			break;
	}

	glPopMatrix();

	glPopAttrib();
}


void Player::receiveDamage( int damage, const QVector3D * position, const QVector3D * direction )
{
	if( !mGodMode )
	{
		if( mArmor >= damage )
		{
			mArmor -= damage;
		}
		else
		{
			int admg = damage - mArmor;
			mArmor -= (damage - admg);

			setLife( life() - admg );

			if( life() <= 0 )
			{
				if( state() == ALIVE )
					setState( DYING );
				setLife( 0 );
			}
		}
	}
}


void Player::giveWeapon( QSharedPointer< AWeapon > weapon )
{
	if( weapon.isNull() )
		return;

	foreach( QSharedPointer<AWeapon> myWeapon, mWeapons )
	{
		if( myWeapon->name() == weapon->name() )
		{
			myWeapon->setAmmo( myWeapon->ammo() + 1 );	// TODO: how much ammo to add
			return;
		}
	}

	mWeapons.append( weapon );
	add( weapon );

	if( mCurrentWeapon )
		mCurrentWeapon->holster();
	mCurrentWeapon = *(--mWeapons.end());
	mCurrentWeapon->pull();
}


void Player::updateRotation( const double & delta )
{
	mAxisRotationY += -mMouseDelta.x()/5.0f;
	mAxisRotationX += mMouseDelta.y()/5.0f;
	if( mAxisRotationX > 80 )
		mAxisRotationX = 80;
	if( mAxisRotationX < -80)
		mAxisRotationX = -80;
	mMouseDelta = QPointF( 0, 0 );
	QQuaternion qX = QQuaternion::fromAxisAndAngle( 1,0,0, mAxisRotationX );
	QQuaternion qY = QQuaternion::fromAxisAndAngle( 0,1,0, mAxisRotationY );
	setRotation( qY * qX );
}


void Player::updatePosition( const double & delta )
{
	QVector3D control( 0.0f, 0.0f, 0.0f );
	float speed;
	if( mForwardPressed )
		control.setZ( control.z() + 1.0f );
	if( mBackwardPressed )
		control.setZ( control.z() - 1.0f );
	if( mLeftPressed )
		control.setX( control.x() + 1.0f );
	if( mRightPressed )
		control.setX( control.x() - 1.0f );

	if( mGodMode )
	{
		if( mUpPressed )
			control.setY( control.y() + 1.0f );
		if( mDownPressed )
			control.setY( control.y() - 1.0f );
		if( mSpeedPressed )
		{
			speed = 300.0;
		} else {
			speed = 50.0;
		}
		QVector3D finalMove( control.x()*left() + control.y()*up() + control.z()*direction() );
		move( finalMove*speed*delta );
	} else {
		control.normalize();
		if( mSpeedPressed && mSpeedCooldown <= 0.0f )
		{
			speed = 15.0;
			mSpeedCounter += delta;
		} else {
			speed = 8.0;
		}

		if( mSpeedCounter >= 3.0f )
		{
			mSpeedPressed = false;
			mSpeedCooldown = 2.0f;
			mSpeedCounter = 0.0f;
		}

		if( mSpeedCooldown > 0.0f )
			mSpeedCooldown -= delta;

		QVector3D finalMove(0,0,0);
		if( mOnGround )
		{
			QVector3D groundDirection = QVector3D::crossProduct( left(), mGroundNormal );
			QVector3D groundLeft = QVector3D::crossProduct( mGroundNormal, groundDirection );
			if( mGroundNormal.y() > 0.7 )
			{	// if the terrain is flat enough, move along the ground
				finalMove += groundDirection * control.z();
				finalMove += groundLeft * control.x();
			} else {
				finalMove += QVector3D::crossProduct( left(), QVector3D(0,1,0) ) * control.z();
				finalMove += left() * control.x();
			}
			finalMove.normalize();
			finalMove -= mGroundNormal * qMin( 0.0f, ((float)QVector3D::dotProduct( finalMove, mGroundNormal )) );
		} else {
			finalMove += QVector3D::crossProduct( left(), QVector3D(0,1,0) ) * control.z();
			finalMove += left() * control.x();
			finalMove.normalize();
		}

		move( finalMove*speed*delta );

		mVelocityY += -80.0f * delta;	// apply gravity

		if( mUpPressed && mOnGround && mGroundNormal.y() > 0.7 )	// jump if key is pressed and player touches ground
			mVelocityY = 20.0f;

		if( mDownPressed )	// duck by lowering the player's height above ground
		{
			mHeightAboveGround = 1.0f;
		} else {
			if( mHeightAboveGround < 2.0f )
				mHeightAboveGround += 10.0f * delta;
		}

		moveY( mVelocityY * delta );
	}

	QVector3D newPosition = position();
	if( !( world()->collideSphere( this, mHeightAboveGround, newPosition, &mGroundNormal ) ).isEmpty() )
	{
		mOnGround = true;
		mGroundNormal.normalize();
		if( mGroundNormal.y() > 0.7 && mVelocityY < 0.0f )
			mVelocityY = 0.0f;
		setPosition( newPosition );
	} else {
		mOnGround = false;
	}
}


void Player::updateTarget( const double & delta )
{
	mTargetDistance = 300.0f;
	if( world()->intersectLine( this, position(), direction(), mTargetDistance, &mTargetNormal ) )
		mTargetAvailable = true;
	else
		mTargetAvailable = false;
	mTarget = position() + direction() * mTargetDistance;
}


void Player::drawCrosshair()
{
	glColor3f( 0.0f, 1.0f, 0.0f );
	glBegin( GL_LINES );
	glVertex3f( 0.0f, 0.05f, -1.0f);
	glVertex3f( 0.0f, 0.1f, -1.0f);
	glVertex3f( 0.0f,-0.05f, -1.0f);
	glVertex3f( 0.0f,-0.1f, -1.0f);
	glVertex3f( 0.05f, 0.0f, -1.0f);
	glVertex3f( 0.15f, 0.0f, -1.0f);
	glVertex3f(-0.05f, 0.0f, -1.0f);
	glVertex3f(-0.15f, 0.0f, -1.0f);
	glEnd();
}
