#include "Player.hpp"

#include <scene/Scene.hpp>
#include <geometry/SplatterSystem.hpp>
#include "World.hpp"
#include "Landscape.hpp"


Player::Player( Scene * scene, World * world ) :
	AObject( scene ),
	mWorld( world )
{
	mForwardPressed = false;
	mLeftPressed = false;
	mBackwardPressed = false;
	mRightPressed = false;
	mUpPressed = false;
	mDownPressed = false;
	mSpeedPressed = false;
	mClipped = true;

	mTarget = QVector3D(0,0,0);

	scene->addKeyListener( this );
	scene->addMouseListener( this );
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
	case Qt::Key_F5:
		mClipped = !mClipped;
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
		mWorld->splatterSystem()->spray( mTarget+QVector3D(0,3,0), 30.0f );
	}
}


void Player::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{

}


void Player::mouseWheelEvent( QGraphicsSceneWheelEvent * event )
{
	mWorld->splatterSystem()->spray( mTarget+QVector3D(0,3,0), 30.0f );
}


void Player::updateSelf( const double & delta )
{
	static float rotateX = 0.0f;
	static float rotateY = 0.0f;
	rotateY += mMouseDelta.x()/5.0f;
	rotateX += -mMouseDelta.y()/5.0f;
	mMouseDelta = QPointF( 0, 0 );
	QQuaternion qX = QQuaternion::fromAxisAndAngle( 1,0,0, rotateX );
	QQuaternion qY = QQuaternion::fromAxisAndAngle( 0,1,0, rotateY );
	setRotation( qX * qY );

	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;
	if( mForwardPressed )
		moveZ += 1.0f;
	if( mBackwardPressed )
		moveZ -= 1.0f;
	if( mLeftPressed )
		moveX += 1.0f;
	if( mRightPressed )
		moveX -= 1.0f;
	if( mUpPressed )
		moveY += 1.0f;
	if( mDownPressed )
		moveY -= 1.0f;
	if( mSpeedPressed )
	{
		moveX *= 300.0*delta;
		moveY *= 300.0*delta;
		moveZ *= 300.0*delta;
	} else {
		moveX *= 50.0*delta;
		moveY *= 50.0*delta;
		moveZ *= 50.0*delta;
	}
	setPosition( position() + moveX*left() + moveY*up() + moveZ*direction() );

	if( mClipped )
	{
		float landscapeHeight;
		if( mWorld->landscape()->terrain()->getHeight( position(), landscapeHeight ) )
		{
			if( position().y() != landscapeHeight + 10 )
				setPositionY( landscapeHeight + 10 );
		}
	}

	float length = 300.0f;
	if( mWorld->getLineIntersection( position(), direction(), length, mTargetNormal ) )
		mTarget = position() + direction() * length;
}

void Player::drawSelf()
{
}

void Player::drawSelfPost()
{
	glDisable( GL_LIGHTING );
	/*
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	*/
	glMatrixMode( GL_MODELVIEW );
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
	/*
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	*/
}
