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

#include "Scene.hpp"

#include "HelpWindow.hpp"
#include "GfxOptionWindow.hpp"
#include "DebugWindow.hpp"
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
	mWireFrame = false;
	mStereo = false;

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
	mEye->setFOV(90);

	mFont = QFont( "Sans", 12, QFont::Normal );

	mDebugWindow = new DebugWindow( this );
	addWidget( mDebugWindow, mDebugWindow->windowFlags() );
	mDebugWindow->move( 64, 64 );
	mDebugWindow->hide();

	mStartMenuWindow = new StartMenuWindow( this );
	addWidget( mStartMenuWindow, mStartMenuWindow->windowFlags() );
	mStartMenuWindow->move( 128, 128 );
	mStartMenuWindow->hide();

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();

	mElapsedTimer.start();

	setMouseGrabbing( true );
}


Scene::~Scene()
{
	delete mEye;
	delete mDebugWindow;
	delete mStartMenuWindow;
	delete mLeftTextureRenderer;
	delete mRightTextureRenderer;
}


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

	glBindTexture( GL_TEXTURE_2D, mLeftTextureRenderer->texID() );
	glDrawArrays( GL_QUADS, 4, 4 );

	glBindTexture( GL_TEXTURE_2D, mRightTextureRenderer->texID() );
	glDrawArrays( GL_QUADS, 8, 4 );

	glBindTexture( GL_TEXTURE_2D, 0 );

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	sQuadVertexBuffer.release();
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

	updateObjects( mDelta );

	if( mStereo )
	{
		mEye->setViewOffset( QVector3D(-0.05,0,0) );
		mEye->setAspect( (float)mLeftTextureRenderer->size().width()/mLeftTextureRenderer->size().height() );
		mLeftTextureRenderer->bind();
		pushAllGL();
			applyDefaultStatesGL();
			glClear( GL_DEPTH_BUFFER_BIT );
			drawObjects();
		popAllGL();
		mLeftTextureRenderer->release();

		mEye->setViewOffset( QVector3D(0.05,0,0) );
		mEye->setAspect( (float)mRightTextureRenderer->size().width()/mRightTextureRenderer->size().height() );
		mRightTextureRenderer->bind();
		pushAllGL();
			applyDefaultStatesGL();
			glClear( GL_DEPTH_BUFFER_BIT );
			drawObjects();
		popAllGL();
		mRightTextureRenderer->release();

		pushAllGL();
			drawStereoFrameBuffers();
		popAllGL();
	}
	else
	{
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


void Scene::secondPassed()
{
	mFramesPerSecond = mFrameCountSecond;
	mFrameCountSecond = 0;
}


void Scene::setMouseGrabbing( bool enable )
{
	//HACK: this fixes mouse movements on entering grabbing mode - unfortunately it may produce stack overflows
	// in QGestureManager::filterEvent(QGraphicsObject*, QEvent*)
	QCursor::setPos( mGLWidget->mapToGlobal( QPoint( width()/2, height()/2 ) ) );
	//HACK: this used to prevent the cursor from beeing visible after enabling grabbing mode when the mouse is on a widget
	//QCoreApplication::processEvents( QEventLoop::AllEvents );

	mMouseGrabbing = enable;
	if( mMouseGrabbing )
		mGLWidget->setCursor( Qt::BlankCursor );
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
		break;
	case Qt::Key_F1:
		mStartMenuWindow->helpWindow()->setVisible( mStartMenuWindow->helpWindow()->isHidden() );
		break;
	case Qt::Key_F2:
		mStartMenuWindow->gfxOptionWindow()->setVisible( mStartMenuWindow->gfxOptionWindow()->isHidden() );
		break;
	case Qt::Key_F3:
		mDebugWindow->setVisible( mDebugWindow->isHidden() );
		break;
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
