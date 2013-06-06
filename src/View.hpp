#ifndef GLVIEW_INCLUDED
#define GLVIEW_INCLUDED

#include <utility/glWrappers.hpp>
#include <utility/alWrappers.hpp>

#include <QGraphicsView>


class QWidget;
class QGraphicsScene;
class QResizeEvent;

class Scene;
class GLWidget;
class Eye;


class View : public QGraphicsView
{
	Q_OBJECT
public:
	View( QWidget * parent = 0 );
	virtual ~View();

	// Overrides:
	Scene * scene() const { return mScene; }

	GLWidget * glWidget() const { return mGLWidget; }

protected:
	// Overrides:
	void resizeEvent( QResizeEvent * event );
	void keyPressEvent( QKeyEvent * event );

private:
	GLWidget * mGLWidget;
	Scene * mScene;
	QWidget * mDebugOptionWindow;
	QWidget * mGfxOptionWindow;
	QWidget * mHelpWindow;

	ALCdevice * mALDevice;
	ALCcontext * mALContext;

	void initGL();
	void initAL();
	void initScene();
};


#endif
