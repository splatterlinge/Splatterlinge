#include "MainWindow.hpp"

#include <View.hpp>


MainWindow::MainWindow( QWidget * parent ) :
	QMainWindow( parent )
{
	QSettings settings;
	restoreGeometry( settings.value("geometry").toByteArray() );

	mView = new View( this );
	this->setCentralWidget( mView );

	connect( QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(saveWindowGeometry()) );
}


MainWindow::~MainWindow()
{
	delete mView;
}


void MainWindow::saveWindowGeometry()
{
	QSettings settings;
	settings.setValue( "geometry", saveGeometry() );
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
