#include <QtGui>

#include "MainWindow.hpp"


int main( int argc, char * argv[] )
{
	QCoreApplication::setOrganizationName( "Ununoctium" );
	QCoreApplication::setApplicationName( "Ununoctium" );

	QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );

	QApplication::setDesktopSettingsAware( false );
	QApplication::setStyle(QStyleFactory::create("Plastique"));

	QApplication app( argc, argv );

	MainWindow window;
	window.resize( 800,600 );
	window.show();

	return app.exec();
}
