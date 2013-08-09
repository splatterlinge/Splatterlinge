/*
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.hpp"

#include <View.hpp>


MainWindow::MainWindow( QWidget * parent ) :
	QMainWindow( parent )
{
	resize( 800, 600 );

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
