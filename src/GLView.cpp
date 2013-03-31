#include "GLView.h"

#include <QResizeEvent>
#include <QRect>


GLView::GLView( QWidget * parent ) : QGraphicsView( parent )
{
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setFrameShape( QFrame::NoFrame );
}


GLView::GLView( QGraphicsScene * scene, QWidget * parent ) : QGraphicsView( scene, parent )
{
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setFrameShape( QFrame::NoFrame );
}

void GLView::resizeEvent( QResizeEvent * event )
{
	if( scene() )
		scene()->setSceneRect( QRect(QPoint(0, 0), event->size()) );
	QGraphicsView::resizeEvent( event );
}
