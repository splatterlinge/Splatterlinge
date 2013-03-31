#ifndef GLVIEW_INCLUDED
#define GLVIEW_INCLUDED

#include <QGraphicsView>


class QWidget;
class QGraphicsScene;
class QResizeEvent;


class GLView : public QGraphicsView
{
	Q_OBJECT
public:
	GLView( QWidget * parent = 0 );
	GLView( QGraphicsScene * scene, QWidget * parent = 0 );

protected:
	void resizeEvent( QResizeEvent * event );
};

#endif
