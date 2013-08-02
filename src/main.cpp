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
