#include "GLView.hpp"

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


GLView::GLView( QGLFormat glFormat, QWidget * parent ) : QGraphicsView( parent )
{
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setFrameShape( QFrame::NoFrame );

	mGLWidget = new GLWidget( glFormat, this );
	setViewport( mGLWidget );

	mScene = new Scene( mGLWidget, this );
	mEye = new Eye( mScene );
	mScene->setEye( mEye );
	mScene->eye()->add( QSharedPointer<AObject>( new World( mScene, "earth" ) ) );
	setScene( mScene );

	mHelpWindow = new HelpWindow();
	scene()->addWidget( mHelpWindow, mHelpWindow->windowFlags() );
	mHelpWindow->move( 32, 32 );

	mGfxOptionWindow = new GfxOptionWindow( mScene );
	scene()->addWidget( mGfxOptionWindow, mGfxOptionWindow->windowFlags() );
	mGfxOptionWindow->move( 64, 64 );
	mGfxOptionWindow->hide();
}


GLView::~GLView()
{
	delete mHelpWindow;
	delete mGfxOptionWindow;
	delete mEye;
	delete mScene;
	delete mGLWidget;
}


void GLView::resizeEvent( QResizeEvent * event )
{
	scene()->setSceneRect( QRect(QPoint(0, 0), event->size()) );
	QGraphicsView::resizeEvent( event );
}


void GLView::keyPressEvent( QKeyEvent * event )
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
	default:
		event->ignore();
		break;
	}
}
