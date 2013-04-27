#include <QGLWidget>
#include <QGLFunctions>


class GLWidget : public QGLWidget, public QGLFunctions
{
	Q_OBJECT
public:
	GLWidget( QWidget * parent = 0 ) : QGLWidget( parent ) {}
	GLWidget( QGLContext * context, QWidget * parent = 0 ) : QGLWidget( context, parent ) {}
	GLWidget( QGLFormat & format, QWidget * parent = 0 ) : QGLWidget( format, parent ) {}

	void initializeGL();
};
