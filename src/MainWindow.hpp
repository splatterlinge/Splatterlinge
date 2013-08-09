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

#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <QApplication>
#include <QMainWindow>
#include <QStyleFactory>
#include <QKeyEvent>
#include <QSettings>


class View;


/// The main window
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow( QWidget * parent = 0 );
	virtual ~MainWindow();
	void toggleFullScreen();

protected:
	// Overrides:
	virtual void keyPressEvent( QKeyEvent * event );

private:
	View * mView;

private slots:
	void saveWindowGeometry();
};


#endif
