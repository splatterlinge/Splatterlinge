#include "GLView.hpp"

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
	mScene->eye()->add( QSharedPointer<AObject>( new World( mScene ) ) );
	setScene( mScene );

	QGraphicsProxyWidget * proxy;
	QWidget * w = new QWidget();
	w->setGeometry( 0, 0, 10, 10 );
	w->setWindowTitle( tr("Ununoctium") );
	w->setWindowOpacity( 0.8 );
	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, w );
	layout->addWidget( new QLabel("W\tForward\nA\tLeft\nS\tBackward\nD\tRight\nControl\tDown\nSpace\tUp\nShift\tSpeedup") );
	w->setLayout( layout );
	proxy = scene()->addWidget( w, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
	proxy->setPos( 32, 32 );
}


GLView::~GLView()
{
	delete mEye;
	delete mScene;
	delete mGLWidget;
}


void GLView::resizeEvent( QResizeEvent * event )
{
	scene()->setSceneRect( QRect(QPoint(0, 0), event->size()) );
	QGraphicsView::resizeEvent( event );
}
