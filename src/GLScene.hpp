#ifndef GLSCENE_INCLUDED
#define GLSCENE_INCLUDED

#include <QtGui>
#include <QtOpenGL>


class QGraphicsItem;
class QGraphicsProxyWidget;
class QGLWidget;

class Terrain;


class GLScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit GLScene( QGLWidget * glWidget, QObject * parent = 0 );

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
	QGLShaderProgram * basicShader;
	GLuint mDiffuseMap1;
	GLuint mSpecularMap1;
	GLuint mNormalMap1;
	GLuint mDiffuseMap2;
	GLuint mSpecularMap2;
	GLuint mNormalMap2;

	bool mForwardPressed;
	bool mBackwardPressed;
	bool mLeftPressed;
	bool mRightPressed;
	bool mUpPressed;
	bool mDownPressed;
	bool mSpeedPressed;
	
	Terrain * mTerrain;

private slots:
	void secondPassed();
};


#endif
