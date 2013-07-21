#ifndef GLVIEW_INCLUDED
#define GLVIEW_INCLUDED

#include <utility/glWrappers.hpp>
#include <utility/alWrappers.hpp>

#include <QGraphicsView>


class QWidget;
class QGraphicsScene;
class QResizeEvent;

class StartMenuWindow;
class DebugWindow;
class HelpWindow;
class GfxOptionWindow;
class Scene;
class GLWidget;
class World;


/// View on a scene
class View : public QGraphicsView
{
	Q_OBJECT
public:
	View( QWidget * parent = 0 );
	virtual ~View();

	GLWidget * glWidget() const { return mGLWidget; }

	// Overrides:
	virtual Scene * scene() const { return mScene; }

protected:
	// Overrides:
	virtual void resizeEvent( QResizeEvent * event );

private:
	GLWidget * mGLWidget;
	Scene * mScene;
	World * mWorld;

	ALCdevice * mALDevice;
	ALCcontext * mALContext;

	void initGL();
	void initAL();
	void initScene();
};


#endif
