#include "GLView.hpp"

#include <QResizeEvent>
#include <QRect>
#include "GLScene.hpp"


GLView::GLView( QGLFormat glFormat, QWidget * parent ) : QGraphicsView( parent )
{
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setFrameShape( QFrame::NoFrame );

	mGLWidget = new QGLWidget( glFormat, this );

	setViewport( mGLWidget );
	setScene( new GLScene( mGLWidget, this ) );

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


void GLView::resizeEvent( QResizeEvent * event )
{
	scene()->setSceneRect( QRect(QPoint(0, 0), event->size()) );
	QGraphicsView::resizeEvent( event );
}
