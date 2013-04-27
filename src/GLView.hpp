#ifndef GLVIEW_INCLUDED
#define GLVIEW_INCLUDED

#include <QGraphicsView>
#include <QGLFormat>


class QWidget;
class QGraphicsScene;
class QResizeEvent;

class GLScene;
class GLWidget;


class GLView : public QGraphicsView
{
	Q_OBJECT
public:
	GLView( QGLFormat glFormat, QWidget * parent = 0 );
	virtual ~GLView();

	// Overrides:
	GLScene * scene() const { return mGLScene; }

	GLWidget * glWidget() const { return mGLWidget; }

protected:
	// Overrides:
	void resizeEvent( QResizeEvent * event );

private:
	GLWidget * mGLWidget;
	GLScene * mGLScene;
};

#endif
