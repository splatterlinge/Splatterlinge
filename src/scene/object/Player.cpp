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
	mGodMode = true;
	mOnGround = false;

	mVelocityY = 0.0f;
	mHeightAboveGround = 2.0f;
	mAxisRotationX = 0.0f;
	mAxisRotationY = 0.0f;

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
		mWorld->splatterSystem()->spray( mTarget+QVector3D(0,3,0), 30.0f );
	}
}


void Player::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{

}


void Player::mouseWheelEvent( QGraphicsSceneWheelEvent * event )
{
	mWorld->splatterSystem()->spray( mTarget+QVector3D(0,3,0), 10.0f );
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

	if( mGodMode )
	{
		QVector3D move( 0.0f, 0.0f, 0.0f );
		if( mForwardPressed )
			move.setZ( move.z() + 1.0f );
		if( mBackwardPressed )
			move.setZ( move.z() - 1.0f );
		if( mLeftPressed )
			move.setX( move.x() + 1.0f );
		if( mRightPressed )
			move.setX( move.x() - 1.0f );
		if( mUpPressed )
			move.setY( move.y() + 1.0f );
		if( mDownPressed )
			move.setY( move.y() - 1.0f );
		if( mSpeedPressed )
		{
			move *= 300.0*delta;
		} else {
			move *= 50.0*delta;
		}
		setPosition( position() + move.x()*left() + move.y()*up() + move.z()*direction() );
	} else {
		QVector3D move( 0.0f, 0.0f, 0.0f );
		if( mForwardPressed )
			move.setZ( move.z() + 1.0f );
		if( mBackwardPressed )
			move.setZ( move.z() - 1.0f );
		if( mLeftPressed )
			move.setX( move.x() + 1.0f );
		if( mRightPressed )
			move.setX( move.x() - 1.0f );
		move.normalize();
		if( mSpeedPressed )
		{
			move *= 20.0*delta;
		} else {
			move *= 10.0*delta;
		}
		QVector3D moveDirection = direction();
		moveDirection.setY( 0.0f );
		QVector3D moveLeft = left();

		QVector3D terrainNormal;
		if( mWorld->landscape()->terrain()->getNormal( position(), terrainNormal ) )
		{
			if( terrainNormal.y() > 0.6 && mOnGround )
			{	// if the terrain is flat enough and the player touches the ground, move along the terrain instead
				moveDirection = QVector3D::crossProduct( left(), terrainNormal );
				moveLeft = QVector3D::crossProduct( terrainNormal, direction() );
			}
		}

		moveDirection.normalize();
		moveLeft.normalize();

		setPosition( position() + move.x()*moveLeft + move.z()*moveDirection );

		mVelocityY += -80.0f * delta;	// apply gravity

		if( mUpPressed && mOnGround )	// jump if key is pressed and player touches ground
			mVelocityY = 20.0f;

		if( mDownPressed )	// duck by lowering the player's height above ground
		{
			mHeightAboveGround = 1.0f;
		} else {
			if( mHeightAboveGround < 2.0f )
				mHeightAboveGround += 10.0f * delta;
		}

		setPositionY( position().y() + mVelocityY * delta );
	}

	float landscapeHeight;
	if( mWorld->landscape()->terrain()->getHeight( position(), landscapeHeight ) )
	{
		if( landscapeHeight + mHeightAboveGround + 0.1f >= position().y() )
		{
			if( landscapeHeight + mHeightAboveGround > position().y() )
			{
				setPositionY( landscapeHeight + mHeightAboveGround );
				if( mVelocityY < 0.0f )
					mVelocityY = 0.0f;
			}
			mOnGround = true;
		} else {
			mOnGround = false;
		}
	}

	float length = 300.0f;
	if( mWorld->getLineIntersection( position(), direction(), length, mTargetNormal ) )
		mTarget = position() + direction() * length;
}

void Player::drawSelf()
{

}

void Player::drawAfterSelf()
{
	glDisable( GL_LIGHTING );
	
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
/*
	glColor3d( 1.0f, 1.0f, 1.0f );
	glDisable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.1 );
	QImage map = QImage( "data/crowbar.png" );
	if( map.isNull() )
	{
		qFatal(
			"Texture could not be loaded!"
		);
	}
	GLuint texture =  scene()->glWidget()->bindTexture( map );
	glBindTexture( GL_TEXTURE_2D, texture );
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f( -0.7f, -0.7f, -1.0f);
	glTexCoord2f(0, 1); glVertex3f( -0.7f, 0.1f, -1.0f);
	glTexCoord2f(1, 1); glVertex3f( 0.0f, 0.1f, -1.0f);
	glTexCoord2f(1, 0); glVertex3f( 0.0f, -0.7f, -1.0f);
	glEnd();
	glDisable( GL_ALPHA_TEST );
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
*/
	glPopAttrib();
}
