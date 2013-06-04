#include "MainWindow.hpp"

#include <GLView.hpp>


MainWindow::MainWindow( QWidget * parent) : QMainWindow( parent )
{
//	QGLFormat glFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering | QGL::SampleBuffers | QGL::NoAccumBuffer );
	QGLFormat glFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::DirectRendering | QGL::NoAccumBuffer );
	glFormat.setVersion( 2, 1 );
//	glFormat.setSwapInterval( 1 );

	mGLView = new GLView( glFormat, this );

	this->setCentralWidget( mGLView );
}


MainWindow::~MainWindow()
{
	delete mGLView;
}


void MainWindow::toggleFullScreen()
{
	if( this->windowState() == Qt::WindowFullScreen )
		this->showNormal();
	else
		this->showFullScreen();
}


void MainWindow::keyPressEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_Return:
		if( event->modifiers() == Qt::AltModifier )
		{
			toggleFullScreen();
		}
		break;
	default:
		event->ignore();
		break;
	}
}
