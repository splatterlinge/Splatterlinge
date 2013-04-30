#include "GLScene.hpp"

#include <QPainter>
#include <GL/glu.h>

#include "GLWidget.hpp"
#include "Landscape.hpp"
#include "Sky.hpp"
#include "TextureRenderer.hpp"
#include "teapot.h"


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

	mTimeOfDay = 0.0f;
	mSky = new Sky( mGLWidget, "earth", &mTimeOfDay );
	mLandscape = new Landscape( mGLWidget, "test" );

	mTeapotMaterial = new Material( mGLWidget, "KirksEntry" );

//	mTexRenderer = new TextureRenderer( mGLWidget, QSize(256,256), true );

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
	delete mSky;
	delete mLandscape;
	delete mTeapotMaterial;
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
	glRotatef( rotateX, 1,0,0 );
	glRotatef( rotateY, 0,1,0 );
	
	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;
	static float posX = 0.0f;
	static float posY = 0.0f;
	static float posZ = 0.0f;
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
	posX += cosf( rotateY*(M_PI/180.0) ) * moveX - sinf( rotateY*(M_PI/180.0) ) * moveZ;
	posZ += sinf( rotateY*(M_PI/180.0) ) * moveX + cosf( rotateY*(M_PI/180.0) ) * moveZ;
	posY += moveY;

	float landscapeHeight = mLandscape->getTerrain()->getHeight( QVector3D(posX,posY,posZ) );
	if( posY < landscapeHeight + 2 )
		posY = landscapeHeight + 2;

	glTranslatef( -posX, -posY, -posZ );

	if( mSpeedPressed )
		mTimeOfDay += 0.002f;
	else
		mTimeOfDay += 0.0002f;

	if( mTimeOfDay > 1.0f )
		mTimeOfDay -= 1.0f;

	mSky->update( mDelta );
	glLightfv( GL_LIGHT0, GL_POSITION, reinterpret_cast<const GLfloat*>(&mSky->sunDirection()) );
	glLightfv( GL_LIGHT0, GL_AMBIENT, reinterpret_cast<const GLfloat*>(&mSky->ambient()) );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, reinterpret_cast<const GLfloat*>(&mSky->diffuse()) );
	glLightfv( GL_LIGHT0, GL_SPECULAR, reinterpret_cast<const GLfloat*>(&mSky->specular()) );
	mSky->draw( QVector3D( posX, posY, posZ ) );

	float viewDistance = 400.0f;
	glFogfv( GL_FOG_COLOR, reinterpret_cast<const GLfloat*>(&mSky->fogColor()) );
	glFogf( GL_FOG_START, viewDistance*0.75f );
	glFogf( GL_FOG_END, viewDistance );
//	glFogf( GL_FOG_DENSITY, 0.01f );

	mTeapotMaterial->bind();
	glPushMatrix();
	glTranslatef( 0, -16, 0 );
	glDisable( GL_CULL_FACE );
	teapot( 20, 16.0f, GL_FILL );
	glEnable( GL_CULL_FACE );
	glPopMatrix();
	mTeapotMaterial->release();

	mLandscape->drawPatch( QRectF( posX-viewDistance, posZ-viewDistance, viewDistance*2, viewDistance*2 ) );

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
	event->accept();
}
