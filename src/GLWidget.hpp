#ifndef GLWIDGET_INCLUDED
#define GLWIDGET_INCLUDED

#include <GL/glew.h>
#include <GL/glu.h>

#include <QGLWidget>



class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLWidget( QWidget * parent = 0 ) :
		QGLWidget( parent )
		{ makeCurrent(); glewInit(); }

	GLWidget( QGLContext * context, QWidget * parent = 0 ) :
		QGLWidget( context, parent )
		{ makeCurrent(); glewInit(); }

	GLWidget( QGLFormat & format, QWidget * parent = 0 ) :
		QGLWidget( format, parent )
		{ makeCurrent(); glewInit(); }
};


#endif
