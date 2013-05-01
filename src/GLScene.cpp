#include "GLScene.hpp"

#include <QPainter>
#include <QTimer>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <GL/glu.h>

#include "GLWidget.hpp"
#include "World.hpp"
//#include "TextureRenderer.hpp"


GLScene::GLScene( GLWidget * glWidget, QObject * parent ) :
	QGraphicsScene( parent ),
	mGLWidget( glWidget )
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

//	mTexRenderer = new TextureRenderer( mGLWidget, QSize(256,256), true );

	mEye = new Eye( this );

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();

	QTimer * updateTimer = new QTimer( this );
	QObject::connect( updateTimer, SIGNAL(timeout()), this, SLOT(update()) );
	updateTimer->setInterval( 10 );
	updateTimer->start();
	
	mElapsedTimer.start();
}


GLScene::~GLScene()
{
	delete mEye;
}


QGraphicsProxyWidget * GLScene::addWidget( QWidget * widget, Qt::WindowFlags wFlags )
{
	QGraphicsProxyWidget * proxy = QGraphicsScene::addWidget( widget, wFlags );
	proxy->setFlag( QGraphicsItem::ItemIsMovable );
	proxy->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
	return proxy;
}


void GLScene::drawBackground( QPainter * painter, const QRectF & rect )
{
	mDelta = (double)mElapsedTimer.restart()/1000.0;

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMatrixMode( GL_TEXTURE );	glPushMatrix();
	glMatrixMode( GL_PROJECTION );	glPushMatrix();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();

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

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0f, (float)width()/height(), 0.1f, 1000.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


	static float rotateX = 0.0f;
	static float rotateY = 0.0f;
	rotateY += mDrag.x()/4.0f;
	rotateX += mDrag.y()/4.0f;
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
		moveX *= 0.2;
		moveY *= 0.2;
		moveZ *= 0.2;
	}
	mEye->setPosition( mEye->position() + moveX*mEye->left() + moveY*mEye->up() + moveZ*mEye->direction() );
	
	mEye->update( mDelta );
	mEye->draw();
/*
	glPushAttrib( GL_VIEWPORT_BIT );
	mTexRenderer->bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport( 0, 0, mTexRenderer->size().width(), mTexRenderer->size().width() );
	mSky->draw( QVector3D( posX, posY, posZ ) );
	mLandscape->drawPatch( QRectF( posX-viewDistance, posZ-viewDistance, viewDistance*2, viewDistance*2 ) );
	mTexRenderer->release();
	glPopAttrib();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, mTexRenderer->texID() );
	glBegin( GL_QUADS );
		glNormal3f(0,1,0);	glMultiTexCoord2f( GL_TEXTURE0, 0, 0 );	glVertex3f(-100, 10, 100 );
		glNormal3f(0,1,0);	glMultiTexCoord2f( GL_TEXTURE0, 1, 0 );	glVertex3f( 100, 10, 100 );
		glNormal3f(0,1,0);	glMultiTexCoord2f( GL_TEXTURE0, 1, 1 );	glVertex3f( 100, 10,-100 );
		glNormal3f(0,1,0);	glMultiTexCoord2f( GL_TEXTURE0, 0, 1 );	glVertex3f(-100, 10,-100 );
	glEnd();
*/
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

	GLenum error = glGetError();
	if( error )
		qWarning() << QString( reinterpret_cast<const char*>(gluErrorString(error)) );
}


void GLScene::secondPassed()
{
	mFramesPerSecond = mFrameCountSecond;
	mFrameCountSecond = 0;
}


void GLScene::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseMoveEvent( event );
	if( event->isAccepted() )
		return;
	if( mDragging )
	{
		mDrag += event->screenPos() - event->lastScreenPos();
		event->accept();
	}
}


void GLScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mousePressEvent( event );
	if( event->isAccepted() )
		return;
	mDragging = true;
}


void GLScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	mDragging = false;
}


void GLScene::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	QGraphicsScene::wheelEvent( event );
}


void GLScene::keyPressEvent( QKeyEvent * event )
{
	QGraphicsScene::keyPressEvent( event );
	if( event->isAccepted() )
		return;
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

	QList< KeyListener* >::iterator i;
	for( i = mKeyListeners.begin(); i != mKeyListeners.end(); ++i )
		(*i)->keyPressEvent( event );

	event->accept();
}


void GLScene::keyReleaseEvent( QKeyEvent * event )
{
	QGraphicsScene::keyReleaseEvent( event );
	if( event->isAccepted() )
		return;
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

	QList< KeyListener* >::iterator i;
	for( i = mKeyListeners.begin(); i != mKeyListeners.end(); ++i )
		(*i)->keyReleaseEvent( event );

	event->accept();
}
