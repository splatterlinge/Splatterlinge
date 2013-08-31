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

#include "View.hpp"

#include <GLWidget.hpp>
#include <scene/Scene.hpp>
#include <scene/object/Eye.hpp>
#include <scene/object/World.hpp>

#include <QBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QGraphicsWidget>
#include <QRect>
#include <QSettings>


void View::initAL()
{
	qDebug( "* OpenAL:" );
//	const ALCchar * extensionsALC = alcGetString( NULL, ALC_EXTENSIONS );
//	qDebug() << "OpenAL: Supported context extensions:" << extensionsALC;
#ifdef ALC_ALL_DEVICES_SPECIFIER
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" ) == AL_TRUE )
	{
		const ALCchar * devicesAL = alcGetString( NULL, ALC_ALL_DEVICES_SPECIFIER );
		if( devicesAL )
		{
			qDebug( "\t* %s:", qPrintable(tr("Available devices")) );
			for( const ALchar * d = devicesAL; *d; d += strlen(d)+1 )
			{
				qDebug( "\t\t* %s", d );
			}
		}
	}
#else
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_EXT") == AL_TRUE )
	{
		const ALCchar * devicesAL = alcGetString( NULL, ALC_DEVICE_SPECIFIER );
		if( devicesAL )
		{
			qDebug( "\t* %s:", qPrintable(tr("Available devices")) );
			for( const ALchar * d = devicesAL; *d; d += strlen(d)+1 )
			{
				qDebug( "\t\t* %s", d );
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
	qDebug( "\t* %s:\t%s", qPrintable(tr("Opening default device")), defaultDeviceNameAL );
	mALDevice = alcOpenDevice( defaultDeviceNameAL );
	if( !mALDevice )
		qFatal( "\t\t! %s!", qPrintable(tr("Could not open audio device")) );
	mALContext = alcCreateContext( mALDevice, NULL );
	alcMakeContextCurrent( mALContext );
	const ALCchar * vendorAL = alGetString( AL_VENDOR );
	qDebug( "\t* %s:\t%s", qPrintable(tr("Vendor")), vendorAL );
	const ALCchar * rendererAL = alGetString( AL_RENDERER );
	qDebug( "\t* %s:\t%s", qPrintable(tr("Renderer")), rendererAL );
	const ALCchar * versionAL = alGetString( AL_VERSION );
	qDebug( "\t* %s:\t%s", qPrintable(tr("Version")) , versionAL );
//	const ALCchar * extensionsAL = alcGetString( NULL, AL_EXTENSIONS );
//	qDebug( "\t* %s:\t%s", qPrintable(tr("Supported extensions")), extensionsAL );

	alDopplerFactor( 0.2f );
}


void View::initGL()
{
	qDebug( "* OpenGL:" );
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setFrameShape( QFrame::NoFrame );

	QGLFormat glFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::DirectRendering );
	glFormat.setVersion( 2, 1 );
//	glFormat.setSwapInterval( 1 );

	QSettings settings;
	glFormat.setSampleBuffers( settings.value( "sampleBuffers", false ).toBool() );

	mGLWidget = new GLWidget( glFormat, this );
	setViewport( mGLWidget );

	const GLubyte * vendorGL = glGetString( GL_VENDOR );
	qDebug( "\t* %s:\t%s", qPrintable(tr("Vendor")), vendorGL );
	const GLubyte * rendererGL = glGetString( GL_RENDERER );
	qDebug( "\t* %s:\t%s", qPrintable(tr("Renderer")), rendererGL );
	const GLubyte * versionGL = glGetString( GL_VERSION );
	qDebug( "\t* %s:\t%s", qPrintable(tr("Version")), versionGL );
	const GLubyte * glslVersionGL = glGetString( GL_SHADING_LANGUAGE_VERSION );
	qDebug( "\t* %s:\t%s", qPrintable(tr("GLSL version")), glslVersionGL );
//	const GLubyte * extensionsGL = glGetString( GL_EXTENSIONS );
//	qDebug( "\t* %s:\t%s", qPrintable(tr("Supported extensions")), extensionsGL );
}


void View::initScene()
{
	QSettings settings;
	Material::setFilterAnisotropy( settings.value( "materialFilterAnisotropy", 1.0f ).toFloat() );
	MaterialQuality::setMaximum( MaterialQuality::fromString(
		settings.value( "materialQuality", MaterialQuality::toString(MaterialQuality::HIGH) ).toString()
	));
	SplatterQuality::setMaximum( SplatterQuality::fromString(
		settings.value( "splatterQuality", SplatterQuality::toString(SplatterQuality::HIGH) ).toString()
	));

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
}


View::~View()
{
	alcMakeContextCurrent( NULL );
	alcDestroyContext( mALContext );
	alcCloseDevice( mALDevice );
	delete mScene->root();
	delete mScene;
	delete mGLWidget;
}


void View::resizeEvent( QResizeEvent * event )
{
	scene()->setSceneRect( QRect(QPoint(0, 0), event->size()) );
	QGraphicsView::resizeEvent( event );
}
