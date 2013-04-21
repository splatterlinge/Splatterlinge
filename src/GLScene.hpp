#ifndef GLSCENE_INCLUDED
#define GLSCENE_INCLUDED

#include <QtGui>
#include <QtOpenGL>

#include "Material.hpp"


class QGraphicsItem;
class QGraphicsProxyWidget;
class QGLWidget;

class Landscape;


class GLScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit GLScene( QGLWidget * glWidget, QObject * parent = 0 );
	~GLScene();

	// Overrides:
	void drawBackground( QPainter * painter, const QRectF & rect );
	QGraphicsProxyWidget * addWidget( QWidget * widget, Qt::WindowFlags wFlags = 0 );

	QGLWidget * glWidget() { return mGLWidget; }

protected:
	// Overrides:
	void keyPressEvent( QKeyEvent * event );
	void keyReleaseEvent( QKeyEvent * event );
	void mousePressEvent( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	void wheelEvent( QGraphicsSceneWheelEvent * wheelEvent );

private:
	QGLWidget * mGLWidget;
	int mFrameCountSecond;
	int mFramesPerSecond;
	QPoint mDrag;
	bool mDragging;
	QFont mFont;

	Material * mTeapotMaterial;

	bool mForwardPressed;
	bool mBackwardPressed;
	bool mLeftPressed;
	bool mRightPressed;
	bool mUpPressed;
	bool mDownPressed;
	bool mSpeedPressed;

	Landscape * mLandscape;

private slots:
	void secondPassed();
};


#endif
