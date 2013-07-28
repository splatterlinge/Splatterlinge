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

	mTarget = QVector3D(0,0,0);
	mDragTeapot = false;

	mWeapons.append( QSharedPointer<Minigun>( new Minigun( world ) ) );
	mWeapons.append( QSharedPointer<Laser>( new Laser( world ) ) );
	mCurrentWeapon = mWeapons.begin();
	add( (*mCurrentWeapon) );

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
		(*mCurrentWeapon)->triggerPressed();
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
		(*mCurrentWeapon)->triggerReleased();
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
	if( event->delta() > 0 && mCurrentWeapon != --mWeapons.end() )
	{
		remove( *mCurrentWeapon );
		++mCurrentWeapon;
		add( *mCurrentWeapon );
	}
	else if( event->delta() < 0 && mCurrentWeapon != mWeapons.begin() )
	{
		remove( *mCurrentWeapon );
		--mCurrentWeapon;
		add( *mCurrentWeapon );
	}
}


void Player::updateSelf( const double & delta )
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

	(*mCurrentWeapon)->setPosition( QVector3D(-0.5f,-0.5f-0.1*QVector3D::dotProduct(QVector3D(0,1,0),direction()), 0.5f ) );

	if( mGodMode )
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
		control.normalize();
		if( mSpeedPressed )
		{
			speed = 20.0;
		} else {
			speed = 10.0;
		}

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
			finalMove -= mGroundNormal * qMin( 0.0, QVector3D::dotProduct( finalMove, mGroundNormal ) );
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
	if( !( world()->collideSphere( this, mHeightAboveGround+0.1f, newPosition, &mGroundNormal ) ).isEmpty() )
	{
		mOnGround = true;
		newPosition = position();
		if( !( world()->collideSphere( this, mHeightAboveGround, newPosition, &mGroundNormal ) ).isEmpty() )
		{
			setPosition( newPosition );
			if( mVelocityY < 0.0f )
				mVelocityY = 0.0f;
		}
	} else {
		mOnGround = false;
	}
}


void Player::update2Self( const double & delta )
{
	float length = 300.0f;
	if( mDragTeapot || mDragTorch )
	{
		if( world()->intersectLine( this, position(), direction(), length, &mTargetNormal ) )
		{
			mTarget = position() + direction() * length;
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

	glPopMatrix();

	glPopAttrib();
}
