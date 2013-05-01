#ifndef GLWIDGET_INCLUDED
#define GLWIDGET_INCLUDED

#include <QGLWidget>
#include <QGLFunctions>

#include <GL/glu.h>


class GLWidget : public QGLWidget, public QGLFunctions
{
	Q_OBJECT
public:
	GLWidget( QWidget * parent = 0 ) :
		QGLWidget( parent ), QGLFunctions()
		{ makeCurrent(); initializeGLFunctions(); }

	GLWidget( QGLContext * context, QWidget * parent = 0 ) :
		QGLWidget( context, parent ), QGLFunctions()
		{ makeCurrent(); initializeGLFunctions(); }

	GLWidget( QGLFormat & format, QWidget * parent = 0 ) :
		QGLWidget( format, parent ), QGLFunctions()
		{ makeCurrent(); initializeGLFunctions(); }
};


#endif
