#include <QApplication>
#include "GLWidget.h"


int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );

	QGLFormat glFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering | QGL::SampleBuffers | QGL::NoAccumBuffer );
	glFormat.setVersion( 2, 1 );
//	glFormat.setSwapInterval( 1 );

	GLWidget window( glFormat );
	window.resize( 800,600 );
	window.show();
	
	return app.exec();
}
