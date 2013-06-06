#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED

#include "object/Eye.hpp"

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QRectF>


class QPainter;
class QGraphicsItem;
class QGraphicsProxyWidget;
class QGLWidget;
class QKeyEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

class GLWidget;
class AMouseListener;
class AKeyListener;


/// Scene manager and interface to Qt
/**
 *
 */
class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit Scene( GLWidget * glWidget, QObject * parent = 0 );
	~Scene();

	// Overrides:
	void drawBackground( QPainter * painter, const QRectF & rect );
	QGraphicsProxyWidget * addWidget( QWidget * widget, Qt::WindowFlags wFlags = 0 );

	GLWidget * glWidget() { return mGLWidget; }
	AObject * root() { return mEye; }
	Eye * eye() { return mEye; }
	void setEye( Eye * eye ) { mEye = eye; }
	void addKeyListener( AKeyListener * listener ) { mKeyListeners.append( listener ); }
	void addMouseListener( AMouseListener * listener ) { mMouseListeners.append( listener ); }
	void removeKeyListener( AKeyListener * listener ) { mKeyListeners.removeOne( listener ); }
	void removeMouseListener( AMouseListener * listener ) { mMouseListeners.removeOne( listener ); }

	void setMouseGrabbing( bool enable );
	bool isMouseGrabbing() const { return mMouseGrabbing; }

	void setWireFrame( bool enable ) { mWireFrame = enable; }

protected:
	// Overrides:
	void keyPressEvent( QKeyEvent * event );
	void keyReleaseEvent( QKeyEvent * event );
	void mousePressEvent( QGraphicsSceneMouseEvent * event );
	void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	void wheelEvent( QGraphicsSceneWheelEvent * event );

private:
	GLWidget * mGLWidget;

	QElapsedTimer mElapsedTimer;
	double mDelta;
	int mFrameCountSecond;
	int mFramesPerSecond;
	QFont mFont;
	bool mWireFrame;

	bool mMouseGrabbing;

	QList<AMouseListener*> mMouseListeners;
	QList<AKeyListener*> mKeyListeners;
	Eye * mEye;

private slots:
	void secondPassed();
};


#endif
