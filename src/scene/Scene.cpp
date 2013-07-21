#include "Scene.hpp"

#include "HelpWindow.hpp"
#include "GfxOptionWindow.hpp"
#include "DebugWindow.hpp"
#include "StartMenuWindow.hpp"

#include "TextureRenderer.hpp"
#include "object/World.hpp"
#include <GLWidget.hpp>
#include <resource/Material.hpp>
#include <utility/glWrappers.hpp>
#include <utility/alWrappers.hpp>

#include <QSettings>
#include <QPainter>
#include <QTimer>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QCoreApplication>


Scene::Scene( GLWidget * glWidget, QObject * parent ) :
	QGraphicsScene( parent ),
	mGLWidget( glWidget ),
	mEye(0)
{
	QSettings settings;

	mRoot = 0;
	mFrameCountSecond = 0;
	mFramesPerSecond = 0;
	mWireFrame = false;

	mMultiSample = settings.value( "sampleBuffers", false ).toBool();

	mEye = new Eye( this );
	mEye->setFarPlane( settings.value( "farPlane", 500.0f ).toFloat() );

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

	mRoot->update( mDelta );
	mRoot->update2( mDelta );

	mEye->applyAL( mDelta );

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMatrixMode( GL_TEXTURE );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();	glLoadIdentity();

	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );

	glClear( GL_DEPTH_BUFFER_BIT );

	if( mWireFrame )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	if( mMultiSample )
		glEnable( GL_MULTISAMPLE );
	else
		glDisable( GL_MULTISAMPLE );

	mEye->applyGL();
	mRoot->draw();
	mRoot->draw2();

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	glMatrixMode( GL_PROJECTION );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );	glPopMatrix();

	glPopAttrib();

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

//	glFlush();
//	glFinish();


	mFrameCountSecond++;

	painter->setPen( QColor(255,255,255) );
	painter->setFont( mFont );
	painter->drawText( rect, Qt::AlignTop | Qt::AlignRight, QString( tr("(%2s) %1 FPS") ).arg(mFramesPerSecond).arg(mDelta) );

	GLenum glError = glGetError();
	if( glError  != GL_NO_ERROR )
		qWarning() << "OpenGL error detected:" << glGetErrorString( glError );

	ALCenum alError = alGetError();
	if( alError != AL_NO_ERROR )
		qWarning() << "OpenAL error detected:" << alGetErrorString( alError );

	mGLWidget->setUpdatesEnabled( true );
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
