#include "MainWindow.h"

#include <QtGui>
#include <QtOpenGL>

#include "GLScene.h"
#include "GLView.h"


MainWindow::MainWindow( QWidget * parent) : QMainWindow( parent )
{
	QGLFormat glFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering | QGL::SampleBuffers | QGL::NoAccumBuffer );
	glFormat.setVersion( 2, 1 );
//	glFormat.setSwapInterval( 1 );

	glScene = new GLScene();

	QGraphicsProxyWidget * proxy;

	QWidget* w = new QWidget();
	w->setGeometry( 0,0,150,150 );
	w->setWindowTitle( tr("Title") );
	w->setWindowOpacity( 0.8 );
	proxy = glScene->addWidget(w);
	proxy->setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint );
	proxy->setPos( 32,32 );

	glView = new GLView( this );
	glView->setViewport( new QGLWidget( glFormat ) );
	glView->setScene( glScene );

	this->setCentralWidget( glView );
}