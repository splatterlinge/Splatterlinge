#ifndef GLWIDGET_INCLUDED
#define GLWIDGET_INCLUDED

#include <QGLWidget>


class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget( QGLFormat & glFormat, QWidget * parent = NULL );

protected:
	void initializeGL();
	void resizeGL( int w, int h );
	void paintGL();
	void mousePressEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
	void keyPressEvent( QKeyEvent * event );

	int frameCount;
	QPoint lastMousePos;
	QPoint drag;

private slots:
	void secondPassed();
};


#endif
