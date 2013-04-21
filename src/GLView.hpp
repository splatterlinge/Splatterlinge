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
	virtual ~GLView();

	// Overrides:
	GLScene * scene() const { return mGLScene; }

	QGLWidget * glWidget() const { return mGLWidget; }

protected:
	// Overrides:
	void resizeEvent( QResizeEvent * event );

private:
	QGLWidget * mGLWidget;
	GLScene * mGLScene;
};

#endif
