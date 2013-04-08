#ifndef GLVIEW_INCLUDED
#define GLVIEW_INCLUDED

#include <QGraphicsView>
#include <QGLFormat>


class QWidget;
class QGraphicsScene;
class QResizeEvent;

class GLScene;


class GLView : public QGraphicsView
{
	Q_OBJECT
public:
	GLView( QGLFormat glFormat, QWidget * parent = 0 );

	// Overrides:
	GLScene * scene() const { return reinterpret_cast<GLScene*>(QGraphicsView::scene()); }

	QGLWidget * glWidget() const { return mGLWidget; }

protected:
	// Overrides:
	void resizeEvent( QResizeEvent * event );

private:
	QGLWidget * mGLWidget;
};

#endif
