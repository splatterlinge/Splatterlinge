#include "View.hpp"

#include "DebugOptionWindow.hpp"
#include "GfxOptionWindow.hpp"
#include "HelpWindow.hpp"

#include <GLWidget.hpp>
#include <scene/Scene.hpp>
#include <scene/object/Eye.hpp>
#include <scene/object/World.hpp>

#include <QBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QRect>


void View::initAL()
{
//	const ALCchar * extensionsALC = alcGetString( NULL, ALC_EXTENSIONS );
//	qDebug() << "OpenAL: Supported context extensions:" << extensionsALC;
#ifdef ALC_ALL_DEVICES_SPECIFIER
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" ) == AL_TRUE )
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
}


void View::initGL()
{
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setFrameShape( QFrame::NoFrame );

	QGLFormat glFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::DirectRendering | QGL::NoAccumBuffer );
	glFormat.setVersion( 2, 1 );
//	glFormat.setSwapInterval( 1 );

	mGLWidget = new GLWidget( glFormat, this );
	setViewport( mGLWidget );

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
}


void View::initScene()
{
	mScene = new Scene( mGLWidget, this );
	mWorld = new World( mScene, "earth" );
	mScene->setRoot( mWorld );
	setScene( mScene );
}


View::View( QWidget * parent ) : QGraphicsView( parent )
{
	initGL();
	initAL();
	initScene();

	mHelpWindow = new HelpWindow();
	scene()->addWidget( mHelpWindow, mHelpWindow->windowFlags() );
	mHelpWindow->move( 32, 32 );

	mGfxOptionWindow = new GfxOptionWindow( mScene );
	scene()->addWidget( mGfxOptionWindow, mGfxOptionWindow->windowFlags() );
	mGfxOptionWindow->move( 64, 64 );
	mGfxOptionWindow->hide();

	mDebugOptionWindow = new DebugOptionWindow( mScene );
	scene()->addWidget( mDebugOptionWindow, mDebugOptionWindow->windowFlags() );
	mDebugOptionWindow->move( 128, 64 );
	mDebugOptionWindow->hide();
}


View::~View()
{
	alcMakeContextCurrent( NULL );
	alcDestroyContext( mALContext );
	alcCloseDevice( mALDevice );
	delete mHelpWindow;
	delete mGfxOptionWindow;
	delete mDebugOptionWindow;
	delete mWorld;
	delete mScene;
	delete mGLWidget;
}


void View::resizeEvent( QResizeEvent * event )
{
	scene()->setSceneRect( QRect(QPoint(0, 0), event->size()) );
	QGraphicsView::resizeEvent( event );
}


void View::keyPressEvent( QKeyEvent * event )
{
	QGraphicsView::keyPressEvent( event );
	switch( event->key() )
	{
	case Qt::Key_F1:
		if( mHelpWindow->isHidden() )
			mHelpWindow->show();
		else
			mHelpWindow->hide();
		break;
	case Qt::Key_F2:
		if( mGfxOptionWindow->isHidden() )
			mGfxOptionWindow->show();
		else
			mGfxOptionWindow->hide();
		break;
	case Qt::Key_F3:
		if( mDebugOptionWindow->isHidden() )
			mDebugOptionWindow->show();
		else
			mDebugOptionWindow->hide();
		break;
	default:
		event->ignore();
		break;
	}
}
