#include "Scene.hpp"

#include "TextureRenderer.hpp"
#include "object/World.hpp"
#include <GLWidget.hpp>
#include <resource/Material.hpp>

#include <QPainter>
#include <QTimer>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>

#include <GL/glu.h>
#include <AL/alc.h>
#include <AL/al.h>


Scene::Scene( GLWidget * glWidget, QObject * parent ) :
	QGraphicsScene( parent ),
	mGLWidget( glWidget ),
	mEye(0)
{
	mFrameCountSecond = 0;
	mFramesPerSecond = 0;
	mDragging = false;

	mForwardPressed = false;
	mLeftPressed = false;
	mBackwardPressed = false;
	mRightPressed = false;
	mUpPressed = false;
	mDownPressed = false;
	mSpeedPressed = false;

	mFont = QFont( "Sans", 12, QFont::Normal );

	mALDevice = alcOpenDevice( NULL );
	if( !mALDevice )
		qFatal( "Could not open audio device!" );
	mALContext = alcCreateContext( mALDevice, NULL );
	alcMakeContextCurrent( mALContext );
	alGetError();

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();
/*
	QTimer * updateTimer = new QTimer( this );
	QObject::connect( updateTimer, SIGNAL(timeout()), this, SLOT(update()) );
	updateTimer->setInterval( 10 );
	updateTimer->start();
*/
	mElapsedTimer.start();
}


Scene::~Scene()
{
	alcMakeContextCurrent( NULL );
	alcDestroyContext( mALContext );
	alcCloseDevice( mALDevice );
}


QGraphicsProxyWidget * Scene::addWidget( QWidget * widget, Qt::WindowFlags wFlags )
{
	QGraphicsProxyWidget * proxy = QGraphicsScene::addWidget( widget, wFlags );
	proxy->setFlag( QGraphicsItem::ItemIsMovable );
	proxy->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
	return proxy;
}


void Scene::drawBackground( QPainter * painter, const QRectF & rect )
{
	mDelta = (double)mElapsedTimer.restart()/1000.0;

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	static float rotateX = 0.0f;
	static float rotateY = 0.0f;
	rotateY += mDrag.x()/5.0f;
	rotateX += mDrag.y()/5.0f;
	mDrag = QPoint( 0, 0 );
	QQuaternion qX = QQuaternion::fromAxisAndAngle( 1,0,0, rotateX );
	QQuaternion qY = QQuaternion::fromAxisAndAngle( 0,1,0, rotateY );
	mEye->setRotation( qX * qY );

	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;
	if( mForwardPressed )
		moveZ -= 1.0f;
	if( mBackwardPressed )
		moveZ += 1.0f;
	if( mLeftPressed )
		moveX -= 1.0f;
	if( mRightPressed )
		moveX += 1.0f;
	if( mUpPressed )
		moveY += 1.0f;
	if( mDownPressed )
		moveY -= 1.0f;
	if( mSpeedPressed )
	{
		moveX *= 3.0;
		moveY *= 3.0;
		moveZ *= 3.0;
	} else {
		moveX *= 0.5;
		moveY *= 0.5;
		moveZ *= 0.5;
	}
	mEye->setPosition( mEye->position() + moveX*mEye->left() + moveY*mEye->up() + moveZ*mEye->direction() );

	mEye->update( mDelta );

	glMatrixMode( GL_TEXTURE );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();	glLoadIdentity();

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glClearColor( 0, 0, 0, 0 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	mEye->draw();
	mEye->drawPostProc();

	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	glMatrixMode( GL_PROJECTION );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );	glPopMatrix();

	glPopAttrib();

//	glFlush();
	glFinish();


	mFrameCountSecond++;

	painter->setPen( QColor(255,255,255) );
	painter->setFont( mFont );
	painter->drawText( rect, Qt::AlignTop | Qt::AlignRight, QString( tr("(%2s) %1 FPS") ).arg(mFramesPerSecond).arg(mDelta) );

	GLenum glError = glGetError();
	if( glError  != GL_NO_ERROR )
		qWarning() << "OpenGL error detected:" << QString( reinterpret_cast<const char*>(gluErrorString(glError)) );

	ALCenum alError = alGetError();
	if( alError != AL_NO_ERROR )
	{
		qWarning() << "OpenAL error detected!";
	}

	emit update();
}


void Scene::secondPassed()
{
	mFramesPerSecond = mFrameCountSecond;
	mFrameCountSecond = 0;
}


void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseMoveEvent( event );
	if( event->isAccepted() )
		return;
	if( mDragging )
	{
		mDrag += event->screenPos() - event->lastScreenPos();
		event->accept();
	}
	
	QList< MouseListener* >::iterator i;
	for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
		(*i)->mouseMoveEvent( event );
}


void Scene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mousePressEvent( event );
	if( event->isAccepted() )
		return;
	mDragging = true;

	QList< MouseListener* >::iterator i;
	for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
		(*i)->mousePressEvent( event );
}


void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	mDragging = false;
	if( event->isAccepted() )
		return;

	QList< MouseListener* >::iterator i;
	for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
		(*i)->mouseReleaseEvent( event );
}


void Scene::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	QGraphicsScene::wheelEvent( event );
	if( event->isAccepted() )
		return;

	QList< MouseListener* >::iterator i;
	for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
		(*i)->wheelEvent( event );
}


void Scene::keyPressEvent( QKeyEvent * event )
{
	QGraphicsScene::keyPressEvent( event );
	if( event->isAccepted() )
		return;

	QList< KeyListener* >::iterator i;
	for( i = mKeyListeners.begin(); i != mKeyListeners.end(); ++i )
		(*i)->keyPressEvent( event );

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
		event->ignore();
		return;
	}
	event->accept();
}


void Scene::keyReleaseEvent( QKeyEvent * event )
{
	QGraphicsScene::keyReleaseEvent( event );
	if( event->isAccepted() )
		return;

	QList< KeyListener* >::iterator i;
	for( i = mKeyListeners.begin(); i != mKeyListeners.end(); ++i )
		(*i)->keyReleaseEvent( event );

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
	default:
		event->ignore();
		return;
	}
	event->accept();
}
