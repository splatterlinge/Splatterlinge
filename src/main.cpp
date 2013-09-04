/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
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

#include <QDir>
#include <QTextCodec>


int main( int argc, char * argv[] )
{
	// first look for data directory
	if( !QDir("data").exists() )
	{
		qWarning( "! %s", qPrintable(QObject::tr("Data directory not found in current working directory! Searching...")) );
		QString newWorkingDirectory;
		QStringList dataSearchPaths;
		dataSearchPaths << "./";
		dataSearchPaths << "../";
		QStringList::const_iterator i;
		for( i = dataSearchPaths.constBegin(); i != dataSearchPaths.constEnd(); ++i )
		{
			QDir dir((*i)+"data");
			if( dir.exists() )
			{
				newWorkingDirectory = (*i);
//				qDebug( "* %s", qPrintable(QObject::tr("Data directory found at \"%1\"").arg(newWorkingDirectory)) );
				break;
			}
		}
		if( newWorkingDirectory.isNull() )
		{
			qFatal( "E %s", qPrintable(QObject::tr("Data directory not found! Tried: \"%1\"!").arg(dataSearchPaths.join("\" \""))) );
		}
		if( !QDir::setCurrent( newWorkingDirectory ) )
		{
			qFatal( "E %s", qPrintable(QObject::tr("Could not change working directory to \"%1\"!").arg(newWorkingDirectory)) );
		}
		qDebug( "* %s", qPrintable(QObject::tr("Changed working directory to \"%1\"").arg(newWorkingDirectory)) );
	}

	// needed for QSettings
	QCoreApplication::setOrganizationName( "Splatterlinge" );
	QCoreApplication::setApplicationName( "Splatterlinge" );

	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );

	QApplication::setDesktopSettingsAware( false );
	QApplication::setStyle( QStyleFactory::create( "Plastique" ) );

	QApplication app( argc, argv );

	MainWindow window;
	window.show();

	return app.exec();
}
