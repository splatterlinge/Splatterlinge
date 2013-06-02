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
#include <QCoreApplication>

#include <GL/glu.h>
#include <AL/alc.h>
#include <AL/al.h>


Scene::Scene( GLWidget * glWidget, QObject * parent ) :
	QGraphicsScene( parent ),
	mGLWidget( glWidget ),
	mEye(0)
{
	mPlayer = new Player( this );
	mFrameCountSecond = 0;
	mFramesPerSecond = 0;
	mWireFrame = false;

	mForwardPressed = false;
	mLeftPressed = false;
	mBackwardPressed = false;
	mRightPressed = false;
	mUpPressed = false;
	mDownPressed = false;
	mSpeedPressed = false;

	mFont = QFont( "Sans", 12, QFont::Normal );

	const GLubyte * vendorGL = glGetString( GL_VENDOR );
	qDebug() << "OpenGL: Vendor:\t" << QString((const char*)vendorGL);
	const GLubyte * rendererGL = glGetString( GL_RENDERER );
	qDebug() << "OpenGL: Renderer:\t" << QString((const char*)rendererGL);
	const GLubyte * versionGL = glGetString( GL_VERSION );
	qDebug() << "OpenGL: Version:\t" << QString((const char*)versionGL);
	const GLubyte * glslVersionGL = glGetString( GL_SHADING_LANGUAGE_VERSION );
	qDebug() << "OpenGL: GLSL version:\t" << QString((const char*)glslVersionGL);
//	const GLubyte * extensionsGL = glGetString( GL_EXTENSIONS );
//	qDebug() << "OpenGL: Supported extensions:" << QString((const char*)extensionsGL);

//	const ALCchar * extensionsALC = alcGetString( NULL, ALC_EXTENSIONS );
//	qDebug() << "OpenAL: Supported context extensions:" << extensionsALC;
#ifdef ALC_ALL_DEVICES_SPECIFIER
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT") == AL_TRUE )
	{
		const ALCchar * devicesAL = alcGetString( NULL, ALC_ALL_DEVICES_SPECIFIER );
		if( devicesAL )
		{
			qDebug() << "OpenAL: Available devices:";
			for( const ALchar * d = devicesAL; *d; d += strlen(d)+1 )
			{
				qDebug() << " *" << d;
			}
		}
	}
#else
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_EXT") == AL_TRUE )
	{
		const ALCchar * devicesAL = alcGetString( NULL, ALC_DEVICE_SPECIFIER );
		if( devicesAL )
		{
			qDebug() << "OpenAL: Available devices:";
			for( const ALchar * d = devicesAL; *d; d += strlen(d)+1 )
			{
				qDebug() << " *" << d;
			}
		}
	}
#endif
	const ALCchar * defaultDeviceNameAL = NULL;
#ifdef ALC_DEFAULT_ALL_DEVICES_SPECIFIER
	defaultDeviceNameAL = alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );
#else
	defaultDeviceNameAL = alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );
#endif
	qDebug() << "OpenAL: Opening default device:\t" << QString(defaultDeviceNameAL);
	mALDevice = alcOpenDevice( defaultDeviceNameAL );
	if( !mALDevice )
		qFatal( "OpenAL: Could not open audio device!" );
	mALContext = alcCreateContext( mALDevice, NULL );
	alcMakeContextCurrent( mALContext );
	const ALCchar * vendorAL = alGetString( AL_VENDOR );
	qDebug() << "OpenAL: Vendor:\t" << QString(vendorAL);
	const ALCchar * rendererAL = alGetString( AL_RENDERER );
	qDebug() << "OpenAL: Renderer:\t" << QString(rendererAL);
	const ALCchar * versionAL = alGetString( AL_VERSION );
	qDebug() << "OpenAL: Version:\t" << QString(versionAL);
//	const ALCchar * extensionsAL = alcGetString( NULL, AL_EXTENSIONS );
//	qDebug() << "OpenAL: Supported extensions:" << extensionsAL;
	alGetError();

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();

	mElapsedTimer.start();

	setMouseGrabbing( true );
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
	mGLWidget->setUpdatesEnabled( false );
	qint64 delta = mElapsedTimer.restart();
	if( delta == 0 )
	{
		qWarning() << "Rendering too fast - cannot compute delta t!";
		delta = 1;
	}
	mDelta = (double)delta/1000.0;

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
		moveX *= 300.0*mDelta;
		moveY *= 300.0*mDelta;
		moveZ *= 300.0*mDelta;
	} else {
		moveX *= 50.0*mDelta;
		moveY *= 50.0*mDelta;
		moveZ *= 50.0*mDelta;
	}
	mEye->setPosition( mEye->position() + moveX*mEye->left() + moveY*mEye->up() + moveZ*mEye->direction() );

	mEye->update( mDelta );

	glMatrixMode( GL_TEXTURE );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();	glLoadIdentity();

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

	glClear( GL_DEPTH_BUFFER_BIT );

	if( mWireFrame )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	mEye->draw();
	mEye->drawPostProc();
	mPlayer->draw();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	glMatrixMode( GL_PROJECTION );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );	glPopMatrix();

	glPopAttrib();

//	glFlush();
//	glFinish();


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

	mGLWidget->setUpdatesEnabled( true );
}


void Scene::secondPassed()
{
	mFramesPerSecond = mFrameCountSecond;
	mFrameCountSecond = 0;
}


void Scene::setMouseGrabbing( bool enable )
{
	mMouseGrabbing = enable;
	if( mMouseGrabbing )
		mGLWidget->setCursor( Qt::BlankCursor );
	else
		mGLWidget->setCursor( Qt::ArrowCursor );
}


void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseMoveEvent( event );
	if( event->isAccepted() )
		return;

	if( isMouseGrabbing() )
	{
		mDrag += event->scenePos() - QPointF( width()/2, height()/2 );
		QCursor::setPos( mGLWidget->mapToGlobal( QPoint( width()/2, height()/2 ) ) );

		QList< AMouseListener* >::iterator i;
		for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
			(*i)->mouseMoveEvent( event );
		event->accept();
	}
	event->ignore();
}


void Scene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mousePressEvent( event );
	if( event->isAccepted() )
		return;

	QList< AMouseListener* >::iterator i;
	for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
		(*i)->mousePressEvent( event );
}


void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	if( event->isAccepted() )
		return;

	QList< AMouseListener* >::iterator i;
	for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
		(*i)->mouseReleaseEvent( event );
}


void Scene::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	QGraphicsScene::wheelEvent( event );
	if( event->isAccepted() )
		return;

	QList< AMouseListener* >::iterator i;
	for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
		(*i)->wheelEvent( event );
}


void Scene::keyPressEvent( QKeyEvent * event )
{
	QGraphicsScene::keyPressEvent( event );
	if( event->isAccepted() )
		return;

	QList< AKeyListener* >::iterator i;
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
	case Qt::Key_Escape:
		setMouseGrabbing( !isMouseGrabbing() );
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

	QList< AKeyListener* >::iterator i;
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
