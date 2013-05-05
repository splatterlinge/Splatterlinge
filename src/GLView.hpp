#ifndef GLVIEW_INCLUDED
#define GLVIEW_INCLUDED


#include <QGraphicsView>
#include <QGLFormat>


class QWidget;
class QGraphicsScene;
class QResizeEvent;

class Scene;
class GLWidget;
class Eye;


class GLView : public QGraphicsView
{
	Q_OBJECT
public:
	GLView( QGLFormat glFormat, QWidget * parent = 0 );
	virtual ~GLView();

	// Overrides:
	Scene * scene() const { return mScene; }

	GLWidget * glWidget() const { return mGLWidget; }

protected:
	// Overrides:
	void resizeEvent( QResizeEvent * event );

private:
	Eye * mEye;
	GLWidget * mGLWidget;
	Scene * mScene;
};

#endif
