/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
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

#include "Scene.hpp"

#include "HelpWindow.hpp"
#include "OptionWindow.hpp"
#include "StartMenuWindow.hpp"

#include "TextureRenderer.hpp"
#include "AMouseListener.hpp"
#include "AKeyListener.hpp"
#include <GLWidget.hpp>
#include <resource/Material.hpp>
#include <resource/Shader.hpp>
#include <utility/glWrappers.hpp>
#include <utility/alWrappers.hpp>

#include <QSettings>
#include <QPainter>
#include <QTimer>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QCoreApplication>
#include <QGLShaderProgram>

#ifdef OVR_ENABLED
#include "OVR.h"
#endif


const GLfloat Scene::sQuadVertices[] =
{	// positions		texcoords
	// FullScreenQuad
	-1.0,  1.0,  0.0,	0.0, 1.0,
	-1.0, -1.0,  0.0,	0.0, 0.0,
	 1.0, -1.0,  0.0,	1.0, 0.0,
	 1.0,  1.0,  0.0,	1.0, 1.0,
	// LeftScreenQuad
	-1.0,  1.0,  0.0,	0.0, 1.0,
	-1.0, -1.0,  0.0,	0.0, 0.0,
	 0.0, -1.0,  0.0,	1.0, 0.0,
	 0.0,  1.0,  0.0,	1.0, 1.0,
	// RightScreenQuad
	 0.0,  1.0,  0.0,	0.0, 1.0,
	 0.0, -1.0,  0.0,	0.0, 0.0,
	 1.0, -1.0,  0.0,	1.0, 0.0,
	 1.0,  1.0,  0.0,	1.0, 1.0
};

QGLBuffer Scene::sQuadVertexBuffer;


Scene::Scene( GLWidget * glWidget, QObject * parent ) :
	QGraphicsScene( parent ),
	mGLWidget( glWidget ),
	mEye( NULL ),
	mLeftTextureRenderer( NULL ),
	mRightTextureRenderer( NULL )
{
	QSettings settings;

	mRoot = 0;
	mFrameCountSecond = 0;
	mFramesPerSecond = 0;
	mPaused = false;
	mWireFrame = false;
	mStereo = false;
	mStereoEyeDistance = 0.1f;
	mStereoUseOVR = settings.value( "stereoUseOVR", false ).toBool();;

	if( !sQuadVertexBuffer.isCreated() )
	{
		sQuadVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
		sQuadVertexBuffer.create();
		sQuadVertexBuffer.bind();
		sQuadVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
		sQuadVertexBuffer.allocate( sQuadVertices, sizeof(sQuadVertices) );
		sQuadVertexBuffer.release();
	}

	mStereo = settings.value( "stereo", false ).toBool();
	if( mStereo )
		resizeStereoFrameBuffers( QSize(400,600) );

	mMultiSample = settings.value( "sampleBuffers", false ).toBool();

	mEye = new Eye( this );
	mEye->setFarPlane( settings.value( "farPlane", 500.0f ).toFloat() );

	mFont = QFont( "Xolonium", 12, QFont::Normal );
	mBlinkingState = false;

#ifdef OVR_ENABLED
	mOVRShader = new Shader( glWidget, "postProc.OVR" );
	OVR::System::Init( OVR::Log::ConfigureDefaultLog(OVR::LogMask_All) );
	mOVRDeviceManager = OVR::DeviceManager::Create();
	if( !mOVRDeviceManager )
	{
		qCritical() << "!!! Could not create OVR Device Manager!";
		mStereoUseOVR = false;
		goto lSkipOVR;
	}
	mOVRHMDDevice = mOVRDeviceManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
	if( !mOVRHMDDevice )
	{
		qCritical() << "!!! Could not create OVR Device!";
		mStereoUseOVR = false;
		goto lSkipOVR;
	}
	if( mOVRHMDDevice->GetDeviceInfo(&mOVRHMDInfo) )
	{
		qDebug() << "* OVR device info:";
		qDebug() << "\t* Display Device Name =" << mOVRHMDInfo.DisplayDeviceName;
		qDebug() << "\t* Interpupillary Distance =" << mOVRHMDInfo.InterpupillaryDistance;
		qDebug() << "\t* Distortion =" << mOVRHMDInfo.DistortionK[0] << mOVRHMDInfo.DistortionK[1] << mOVRHMDInfo.DistortionK[2] << mOVRHMDInfo.DistortionK[3];
	}
	mOVRSensorDevice = mOVRHMDDevice->GetSensor();
	if( !mOVRSensorDevice )
	{
		qCritical() << "!!! Could not get OVR Sensor!";
		mStereoUseOVR = false;
		goto lSkipOVR;
	}
	mOVRSensorFusion.AttachToSensor( mOVRSensorDevice );
lSkipOVR:
#endif

	mOptionWindow = new OptionWindow( this );
	addWidget( mOptionWindow );
	mOptionWindow->move( 64, 64 );
	mOptionWindow->hide();

	mStartMenuWindow = new StartMenuWindow( this, mOptionWindow );
	addWidget( mStartMenuWindow, mStartMenuWindow->windowFlags() );
	mStartMenuWindow->move( 20, height()/2 );
	mStartMenuWindow->hide();

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();

	QTimer * halfSecondTimer = new QTimer( this );
	QObject::connect( halfSecondTimer, SIGNAL(timeout()), this, SLOT(halfSecondPassed()) );
	halfSecondTimer->setInterval( 500 );
	halfSecondTimer->start();

	mElapsedTimer.start();

	setMouseGrabbing( true );
}


Scene::~Scene()
{
#ifdef OVR_ENABLED
	delete mOVRShader;
#endif
	delete mEye;
	delete mLeftTextureRenderer;
	delete mRightTextureRenderer;
}


#ifdef OVR_ENABLED
QQuaternion Scene::OVROrientation()
{
	OVR::Quatf hmdOrient = mOVRSensorFusion.GetOrientation();
	return QQuaternion( -hmdOrient.z, -hmdOrient.y, -hmdOrient.x, -hmdOrient.w ) * QQuaternion::fromAxisAndAngle(0,0,1,-180);
}
#endif


QGraphicsProxyWidget * Scene::addWidget( QWidget * widget, Qt::WindowFlags wFlags )
{
	QGraphicsProxyWidget * proxy = QGraphicsScene::addWidget( widget, wFlags );
	proxy->setFlag( QGraphicsItem::ItemIsMovable );
	proxy->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
	return proxy;
}


void Scene::applyDefaultStatesGL()
{
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );

	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );

	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );

	glShadeModel( GL_SMOOTH );
	glEnable( GL_LIGHTING );

	glDisable( GL_NORMALIZE );
	glDisable( GL_AUTO_NORMAL );

	glColor4f( 1, 1, 1, 1 );
	glClearColor( 0, 0, 0, 0 );

	if( mWireFrame )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	} else {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}

	if( mMultiSample )
	{
		glEnable( GL_MULTISAMPLE );
	} else {
		glDisable( GL_MULTISAMPLE );
	}
}


void Scene::pushAllGL()
{
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMatrixMode( GL_TEXTURE );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();	glLoadIdentity();
}


void Scene::popAllGL()
{
	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	glMatrixMode( GL_PROJECTION );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );	glPopMatrix();
	glPopAttrib();
}


void Scene::updateObjects( const double & delta )
{
	mRoot->update( delta );
	mRoot->update2( delta );
	mEye->update( delta );
	mEye->applyAL();
}


void Scene::drawObjects()
{
	mEye->applyGL();
	mRoot->draw();
	mRoot->draw2();
}


void Scene::drawStereoFrameBuffers()
{
	sQuadVertexBuffer.bind();
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 5*sizeof(GLfloat), (void*)0 );
	glTexCoordPointer( 2, GL_FLOAT, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)) );

	glDisable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glColor4f( 1, 1, 1, 1 );
	glActiveTexture( GL_TEXTURE0 );
	glClientActiveTexture( GL_TEXTURE0 );

#ifdef OVR_ENABLED
	if( mStereoUseOVR )
	{
		float lensCenter = 0.5f + mOVRLensViewportShift * 0.25f;
		float scale = 0.5f * (1.0f / mOVRDistortionScale);
		mOVRShader->bind();
		mOVRShader->program()->setUniformValue( "lensCenter", QVector2D( lensCenter, 0.5 ) );
		mOVRShader->program()->setUniformValue( "scale", QVector2D( scale, scale ) );
		mOVRShader->program()->setUniformValue( "scaleIn", QVector2D( 2, 2 ) );
//		QVector4D distortion7Inch( 1.0, 0.22, 0.24, 0.0 );
//		QVector4D distortion( 1.0, 0.18, 0.115, 0.0 );
		mOVRShader->program()->setUniformValue( "hmdWarpParam", QVector4D(mOVRHMDInfo.DistortionK[0],mOVRHMDInfo.DistortionK[1],mOVRHMDInfo.DistortionK[2],mOVRHMDInfo.DistortionK[3]) );
//		QVector4D chromAb( 0.996, -0.004, 1.014, 0 );
		mOVRShader->program()->setUniformValue( "chromAbParam", QVector4D(mOVRHMDInfo.ChromaAbCorrection[0],mOVRHMDInfo.ChromaAbCorrection[1],mOVRHMDInfo.ChromaAbCorrection[2],mOVRHMDInfo.ChromaAbCorrection[3]) );
		mOVRShader->program()->setUniformValue( "sourceMap", 0 );
	}
#endif

	glBindTexture( GL_TEXTURE_2D, mLeftTextureRenderer->texID() );
	glDrawArrays( GL_QUADS, 4, 4 );

#ifdef OVR_ENABLED
	if( mStereoUseOVR )
	{
		float lensCenter = 0.5f - mOVRLensViewportShift * 0.25f;
		mOVRShader->program()->setUniformValue( "lensCenter", QVector2D( lensCenter, 0.5 ) );
	}
#endif

	glBindTexture( GL_TEXTURE_2D, mRightTextureRenderer->texID() );
	glDrawArrays( GL_QUADS, 8, 4 );

#ifdef OVR_ENABLED
	if( mStereoUseOVR )
	{
		mOVRShader->release();
	}
#endif

	glBindTexture( GL_TEXTURE_2D, 0 );

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	sQuadVertexBuffer.release();
}


void Scene::drawBackground( QPainter * painter, const QRectF & rect )
{
	mGLWidget->setUpdatesEnabled( false );

	qint64 delta = mElapsedTimer.nsecsElapsed();
	mElapsedTimer.restart();
	if( delta == 0 )
		delta = 1;
	mDelta = (double)delta/1000000000.0;

	if( !mPaused )
		updateObjects( mDelta );

	if( mStereo )
	{
#ifdef OVR_ENABLED
		if( mStereoUseOVR )
		{
			float aspect = float(mOVRHMDInfo.HResolution*0.5f) / float(mOVRHMDInfo.VResolution);
			mOVRLensViewportShift = OVRLensViewportShift( mOVRHMDInfo );
			mOVRDistortionScale = OVRDistortionScale( mOVRHMDInfo, mOVRLensViewportShift, aspect, -1, 0 );

			float halfScreenDistance = mOVRHMDInfo.VScreenSize * 0.5f * mOVRDistortionScale;
			float viewCenter = mOVRHMDInfo.HScreenSize * 0.25f;
			float eyeProjectionShift = viewCenter - mOVRHMDInfo.LensSeparationDistance * 0.5f;
			float projectionCenterOffset = 4.0f * eyeProjectionShift / mOVRHMDInfo.HScreenSize;
			float halfIPD = mOVRHMDInfo.InterpupillaryDistance * 0.5f;

			mEye->setAspect( aspect );
			mEye->setFOV( (2.0f * atanf( halfScreenDistance / mOVRHMDInfo.EyeToScreenDistance )) * (180.0f/M_PI) );
//			mEye->setViewOffset( QVector3D(0,0,0) );

			mEye->setPerspectiveOffset( QVector3D(-projectionCenterOffset,0,0) );
			mEye->setViewOffset( QVector3D(-halfIPD,0,0) );
			mLeftTextureRenderer->bind();
			pushAllGL();
				applyDefaultStatesGL();
				glClear( GL_DEPTH_BUFFER_BIT );
				drawObjects();
			popAllGL();
			mLeftTextureRenderer->release();

			mEye->setPerspectiveOffset( QVector3D(projectionCenterOffset,0,0) );
			mEye->setViewOffset( QVector3D(halfIPD,0,0) );
			mRightTextureRenderer->bind();
			pushAllGL();
				applyDefaultStatesGL();
				glClear( GL_DEPTH_BUFFER_BIT );
				drawObjects();
			popAllGL();
			mRightTextureRenderer->release();
		}
		else
		{
#endif
			mEye->setFOV( 90.0f );
			mEye->setPerspectiveOffset( QVector3D(0,0,0) );

			mEye->setViewOffset( QVector3D(-mStereoEyeDistance/2.0f,0,0) );
			mEye->setAspect( (float)mLeftTextureRenderer->size().width()/mLeftTextureRenderer->size().height() );
			mLeftTextureRenderer->bind();
			pushAllGL();
				applyDefaultStatesGL();
				glClear( GL_DEPTH_BUFFER_BIT );
				drawObjects();
			popAllGL();
			mLeftTextureRenderer->release();

			mEye->setViewOffset( QVector3D(mStereoEyeDistance/2.0f,0,0) );
			mEye->setAspect( (float)mRightTextureRenderer->size().width()/mRightTextureRenderer->size().height() );
			mRightTextureRenderer->bind();
			pushAllGL();
				applyDefaultStatesGL();
				glClear( GL_DEPTH_BUFFER_BIT );
				drawObjects();
			popAllGL();
			mRightTextureRenderer->release();
#ifdef OVR_ENABLED
		}
#endif
		pushAllGL();
			drawStereoFrameBuffers();
		popAllGL();
	}
	else
	{
		mEye->setFOV( 90.0f );
		mEye->setPerspectiveOffset( QVector3D(0,0,0) );
		mEye->setViewOffset( QVector3D(0,0,0) );
		mEye->setAspect( (float)width()/height() );
		pushAllGL();
			applyDefaultStatesGL();
			glClear( GL_DEPTH_BUFFER_BIT );
			drawObjects();
		popAllGL();
	}

	mFrameCountSecond++;
	drawFPS( painter, rect );
	drawHUD( painter, rect );

	GLenum glError = glGetError();
	if( glError  != GL_NO_ERROR )
		qWarning() << "OpenGL error detected:" << glGetErrorString( glError );

	ALCenum alError = alGetError();
	if( alError != AL_NO_ERROR )
		qWarning() << "OpenAL error detected:" << alGetErrorString( alError );

	mGLWidget->setUpdatesEnabled( true );
}


void Scene::drawFPS( QPainter * painter, const QRectF & rect )
{
	painter->setPen( QColor(255,255,255) );
	painter->setFont( mFont );
	painter->drawText( rect, Qt::AlignTop | Qt::AlignRight, QString( tr("(%2s) %1 FPS") ).arg(mFramesPerSecond).arg(mDelta) );
}


void Scene::drawHUD( QPainter * painter, const QRectF & rect )
{
	World * world = dynamic_cast<World*>(mRoot);
	if( !world )
		return;
	QSharedPointer<Player> player = world->player();

	painter->setFont( mFont );
	painter->setRenderHints(
		QPainter::Antialiasing |
		QPainter::SmoothPixmapTransform |
		QPainter::TextAntialiasing |
		QPainter::HighQualityAntialiasing );

	// multi
	float w1 = rect.width()/10*1;
	float w2 = rect.width()/10*2;
	float w4 = rect.width()/10*4;
	float w5 = rect.width()/10*5;
	int h = 5;
	int x = rect.left();
	int y = rect.top();
	QRect multi1Rect( x+w5+w4, y, w1, h );
	QRect multi2Rect( x+w5+w2, y, w2, h );
	QRect multi5Rect( x+w5, y, w2, h );
	QRect multi10Rect( x, y, w5+1, h );
	painter->setPen( QColor(255,255,255,0) );
	painter->setBrush( QBrush( QColor(255,255,255,50) ) );
	painter->drawRect( multi1Rect );
	painter->drawRect( multi2Rect );
	painter->drawRect( multi5Rect );
	painter->drawRect( multi10Rect );

	if( player->killTime() < 10.0f )
	{
//		qDebug() << player->killTime();

		if( player->killTime() < 1.0f )
		{
			painter->setBrush( QBrush( QColor(255,0,0,200) ) );
			painter->drawRect( multi1Rect.left(), multi1Rect.top(), w1-qMin( qMax( player->killTime(), 0.0f ), 1.0f )*w1, multi1Rect.height() );
		}
		if( player->killTime() < 3.0f )
		{
			painter->setBrush( QBrush( QColor(255,255,0,200) ) );
			painter->drawRect( multi2Rect.left(), multi2Rect.top(), w2-qMin( qMax( player->killTime()-1.0f, 0.0f ), 2.0f )*w1, multi2Rect.height() );
		}
		if( player->killTime() < 5.0f )
		{
			painter->setBrush( QBrush( QColor(0,255,0,200) ) );
			painter->drawRect( multi5Rect.left(), multi5Rect.top(), w2-qMin( qMax( player->killTime()-3.0f, 0.0f ), 2.0f )*w1, multi5Rect.height() );
		}
		if( player->killTime() < 10.0f )
		{
			qDebug() << w5 << qMin( qMax( player->killTime()-5.0f, 0.0f ), 2.0f )*w1;
			painter->setBrush( QBrush( QColor(0,0,255,200) ) );
			painter->drawRect( multi10Rect.left(), multi10Rect.top(), w5-qMin( qMax( player->killTime()-5.0f, 0.0f ), 5.0f )*w1, multi10Rect.height() );
		}
	}

	// points
	QRect pointsRect( rect.left()+10, rect.top()+10, 200, 30 );
	painter->setPen( QColor(255,255,255,200) );
	painter->drawText( pointsRect,
		Qt::AlignLeft | Qt::AlignTop,
					   QString( tr("Points: %1").arg(player->points())) );

	// timer
	QRect timerRect( rect.width()/2-100, 10, 200, 30 );
	painter->setPen( QColor(255,255,255,200) );
	int time = player->time();
	int mm = time / 60;
	int ss = time % 60;
	QFont old = painter->font();
	QFont f = painter->font();
	f.setPointSize(24);
	painter->setFont(f);
	painter->drawText( timerRect,
		Qt::AlignCenter | Qt::AlignTop,
		QString( tr("%1:%2").
				 arg(QString::number(mm), 2, '0').
				 arg(QString::number(ss), 2, '0') ) );
	painter->setFont(old);

	// player armor
	QRect armorRect( rect.left()+10, rect.bottom()-74, 100*3, 30 );
	painter->setPen( QColor(0,0,0,0) );
	painter->setBrush( QBrush( QColor(11,110,240,80) ) );
	painter->drawRect( armorRect );
	painter->setBrush( QBrush( QColor(26,121,245,200) ) );
	painter->drawRect( armorRect.left(), armorRect.top(), player->armor()*3, armorRect.height() );
	painter->setPen( QColor(255,255,255,255) );
	painter->drawText( armorRect,
		Qt::AlignCenter | Qt::AlignHCenter,
		QString( tr("%1%").arg(player->armor()) ) );

	// player health
	QRect healthRect( rect.left()+10, rect.bottom()-40, 100*3, 30 );
	painter->setPen( QColor(0,0,0,0) );
	painter->setBrush( QBrush( QColor(255,14,14,80) ) );
	painter->drawRect( healthRect );
	painter->setBrush( QBrush( QColor(230,0,0,200) ) );
	painter->drawRect( healthRect.left(), healthRect.top(), player->life()*3, healthRect.height() );
	painter->setPen( QColor(255,255,255,255) );
	painter->drawText( healthRect,
		Qt::AlignCenter | Qt::AlignHCenter,
		QString( tr("%1%").arg(player->life()) ) );

	// weapon status
	QSharedPointer<AWeapon> weapon = player->currentWeapon();
	if( !weapon.isNull() )
	{
		QRect weaponNameRect( rect.right()-210, rect.bottom()-75, 200, 30 );
		QRect weaponStatusRect( rect.right()-210, rect.bottom()-40, 200, 30 );
		painter->setPen( QColor(0,0,0,0) );
		painter->setBrush( QBrush( QColor(11,110,240,80) ) );
		painter->drawRect( weaponNameRect );
		painter->drawRect( weaponStatusRect );
		painter->setPen( QColor(255,255,255,255) );
		painter->drawText( weaponNameRect,
			Qt::AlignCenter | Qt::AlignHCenter,
			QString( tr("%1").arg(player->currentWeapon()->name()) ) );
		if( player->currentWeapon()->clipammo() == 0 )
		{
			if( !mBlinkingState )
			{
				painter->drawText( weaponStatusRect,
					Qt::AlignCenter | Qt::AlignHCenter,
					QString( tr("%2 | %3 ")
						.arg(player->currentWeapon()->clipammo())
						.arg(player->currentWeapon()->ammo()) ) );
			}
		}
		else
		{
			painter->drawText( weaponStatusRect,
				Qt::AlignCenter | Qt::AlignHCenter,
				QString( tr("%2 | %3 ")
					.arg(player->currentWeapon()->clipammo())
					.arg(player->currentWeapon()->ammo()) ) );
		}
	}
}


void Scene::secondPassed()
{
	mFramesPerSecond = mFrameCountSecond;
	mFrameCountSecond = 0;
}


void Scene::halfSecondPassed()
{
	mBlinkingState = !mBlinkingState;
}


void Scene::setMouseGrabbing( bool enable )
{
	//HACK: this fixes mouse movements on entering grabbing mode - unfortunately it may produce stack overflows
	// in QGestureManager::filterEvent( QGraphicsObject *, QEvent * )
	QCursor::setPos( mGLWidget->mapToGlobal( QPoint( width()/2, height()/2 ) ) );
	//HACK: this used to prevent the cursor from being visible after enabling grabbing mode when the mouse is on a widget
	//QCoreApplication::processEvents( QEventLoop::AllEvents );

	mMouseGrabbing = enable;
	if( mMouseGrabbing )
	{
		mGLWidget->setCursor( Qt::BlankCursor );
	}
	else
		mGLWidget->setCursor( Qt::ArrowCursor );
}


void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	if( isMouseGrabbing() )
	{
		QPointF delta = event->scenePos() - QPoint( width()/2, height()/2 );
		if( !delta.isNull() )
		{
			MouseMoveEvent mouseMoveEvent( delta );
			QList< AMouseListener* >::iterator i;
			for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
				(*i)->mouseMoveEvent( &mouseMoveEvent );
			QCursor::setPos( mGLWidget->mapToGlobal( QPoint( width()/2, height()/2 ) ) );
		}
		event->accept();
	}

	QGraphicsScene::mouseMoveEvent( event );
}


void Scene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if( isMouseGrabbing() )
	{
		QList< AMouseListener* >::iterator i;
		for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
			(*i)->mousePressEvent( event );
		event->accept();
	}

	QGraphicsScene::mousePressEvent( event );
}


void Scene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	if( isMouseGrabbing() )
	{
		QList< AMouseListener* >::iterator i;
		for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
			(*i)->mousePressEvent( event );
		event->accept();
	}

	QGraphicsScene::mouseDoubleClickEvent( event );
}


void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if( isMouseGrabbing() )
	{
		QList< AMouseListener* >::iterator i;
		for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
			(*i)->mouseReleaseEvent( event );
		event->accept();
	}

	QGraphicsScene::mouseReleaseEvent(event);
}


void Scene::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	if( isMouseGrabbing() )
	{
		QList< AMouseListener* >::iterator i;
		for( i = mMouseListeners.begin(); i != mMouseListeners.end(); ++i )
			(*i)->mouseWheelEvent( event );
		event->accept();
	}

	QGraphicsScene::wheelEvent( event );
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
	case Qt::Key_Escape:
		mStartMenuWindow->setVisible( mMouseGrabbing );
		setMouseGrabbing( !mMouseGrabbing );
		setPaused( !mMouseGrabbing );
		break;
#ifdef OVR_ENABLED
	case Qt::Key_F12:
		mOVRSensorFusion.Reset();
		break;
#endif
	default:
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
}


void Scene::setSceneRect( const QRectF & rect )
{
	QGraphicsScene::setSceneRect( rect );
	resizeStereoFrameBuffers( rect.size().toSize() );

	mStartMenuWindow->move( 50, rect.height()/2-mStartMenuWindow->height()/2 );
	mOptionWindow->move( 50+mStartMenuWindow->width()+50, rect.height()/2-mOptionWindow->height()/2 );
}


void Scene::resizeStereoFrameBuffers( const QSize & screenSize )
{
	delete mLeftTextureRenderer;
	delete mRightTextureRenderer;
	mLeftTextureRenderer = NULL;
	mRightTextureRenderer = NULL;

	if( mStereo )
	{
		QSize textureRendererSize = QSize( screenSize.width()/2, screenSize.height() );
		if( textureRendererSize.width() < 1 )
			textureRendererSize.setWidth( 1 );
		if( textureRendererSize.height() < 1 )
			textureRendererSize.setHeight( 1 );
		mLeftTextureRenderer = new TextureRenderer( mGLWidget, textureRendererSize, true );
		mRightTextureRenderer = new TextureRenderer( mGLWidget, textureRendererSize, true );
	}
}
