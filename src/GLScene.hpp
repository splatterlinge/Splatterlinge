#ifndef GLSCENE_INCLUDED
#define GLSCENE_INCLUDED

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
class Landscape;
class Sky;
class Material;
class TextureRenderer;


class GLScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit GLScene( GLWidget * glWidget, QObject * parent = 0 );
	~GLScene();

	// Overrides:
	void drawBackground( QPainter * painter, const QRectF & rect );
	QGraphicsProxyWidget * addWidget( QWidget * widget, Qt::WindowFlags wFlags = 0 );

	GLWidget * glWidget() { return mGLWidget; }

protected:
	// Overrides:
	void keyPressEvent( QKeyEvent * event );
	void keyReleaseEvent( QKeyEvent * event );
	void mousePressEvent( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	void wheelEvent( QGraphicsSceneWheelEvent * wheelEvent );

private:
	GLWidget * mGLWidget;
	QElapsedTimer mElapsedTimer;
	float mDelta;
	int mFrameCountSecond;
	int mFramesPerSecond;
	QPoint mDrag;
	bool mDragging;
	QFont mFont;

	Material * mTeapotMaterial;
	
	TextureRenderer * mTexRenderer;

	bool mForwardPressed;
	bool mBackwardPressed;
	bool mLeftPressed;
	bool mRightPressed;
	bool mUpPressed;
	bool mDownPressed;
	bool mSpeedPressed;

	float mTimeOfDay;
	Sky * mSky;
	Landscape * mLandscape;

private slots:
	void secondPassed();
};


#endif
