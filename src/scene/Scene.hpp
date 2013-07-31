#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED

#include "object/Eye.hpp"

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QRectF>
#include <QGLBuffer>


class QPainter;
class QGraphicsItem;
class QGraphicsProxyWidget;
class QGLWidget;
class QKeyEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

class GLWidget;
class DebugWindow;
class StartMenuWindow;
class AMouseListener;
class AKeyListener;
class TextureRenderer;
class Shader;


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
	void setSceneRect( const QRectF & rect );
	void setSceneRect( qreal x, qreal y, qreal w, qreal h ) { setSceneRect( QRectF( x, y, w, h ) ); }

	GLWidget * glWidget() { return mGLWidget; }

	AObject * root() const { return mRoot; }
	void setRoot( AObject * root ) { mRoot = root; }

	Eye * eye() const { return mEye; }
	void setEye( Eye * eye ) { mEye = eye; }

	void addKeyListener( AKeyListener * listener ) { mKeyListeners.append( listener ); }
	void addMouseListener( AMouseListener * listener ) { mMouseListeners.append( listener ); }
	void removeKeyListener( AKeyListener * listener ) { mKeyListeners.removeOne( listener ); }
	void removeMouseListener( AMouseListener * listener ) { mMouseListeners.removeOne( listener ); }

	void setMouseGrabbing( bool enable );
	bool isMouseGrabbing() const { return mMouseGrabbing; }

	void setWireFrame( bool enable ) { mWireFrame = enable; }
	bool wireFrame() const { return mWireFrame; }
	void setMultiSample( bool enable ) { mMultiSample = enable; }
	bool multiSample() const { return mMultiSample; }

	StartMenuWindow * startMenuWindow() { return mStartMenuWindow; }
	DebugWindow * debugWindow() { return mDebugWindow; }

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
	static const GLfloat sQuadVertices[];
	static QGLBuffer sQuadVertexBuffer;

	GLWidget * mGLWidget;

	DebugWindow * mDebugWindow;
	StartMenuWindow * mStartMenuWindow;

	QElapsedTimer mElapsedTimer;
	double mDelta;
	int mFrameCountSecond;
	int mFramesPerSecond;
	QFont mFont;
	bool mWireFrame;
	bool mMultiSample;

	bool mMouseGrabbing;

	QList<AMouseListener*> mMouseListeners;
	QList<AKeyListener*> mKeyListeners;
	Eye * mEye;
	AObject * mRoot;

	TextureRenderer * mTextureRenderer;
	Shader * mPostProcShader;
	int mPostProcShader_sourceMap;

private slots:
	void secondPassed();
};


#endif
