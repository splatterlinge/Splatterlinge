#include "MainWindow.h"

#include <QtGui>
#include "GLScene.h"
#include "GLView.h"


MainWindow::MainWindow( QWidget * parent) : QMainWindow( parent )
{
	QGLFormat glFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering | QGL::SampleBuffers | QGL::NoAccumBuffer );
	glFormat.setVersion( 2, 1 );
//	glFormat.setSwapInterval( 1 );

	mGLView = new GLView( glFormat, this );

	this->setCentralWidget( mGLView );
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
