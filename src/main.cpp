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

#include <QtGui>

#include "MainWindow.hpp"


int main( int argc, char * argv[] )
{
	// needed for QSettings
	QCoreApplication::setOrganizationName( "Ununoctium" );
	QCoreApplication::setApplicationName( "Ununoctium" );

	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );

	QApplication::setDesktopSettingsAware( false );
	QApplication::setStyle( QStyleFactory::create( "Plastique" ) );

	QApplication app( argc, argv );

	MainWindow window;
	window.show();

	return app.exec();
}
