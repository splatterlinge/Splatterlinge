#include "StartMenuWindow.hpp"

#include "GfxOptionWindow.hpp"
#include "HelpWindow.hpp"

#include <scene/Scene.hpp>

#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>


StartMenuWindow::StartMenuWindow( Scene * scene, QWidget * parent ) :
	QWidget( parent ),
	mScene( scene )
{
	setWindowTitle( tr( "Start Menu" ) );
	setWindowOpacity( 0.8 );

	mHelpWindow = new HelpWindow();
	mScene->addWidget( mHelpWindow, mHelpWindow->windowFlags() );
	mHelpWindow->move( 32, 32 );

	mGfxOptionWindow = new GfxOptionWindow( mScene );
	mScene->addWidget( mGfxOptionWindow, mGfxOptionWindow->windowFlags() );
	mGfxOptionWindow->move( 32, 32 );
	mGfxOptionWindow->hide();

	initMenu();

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint );
}


StartMenuWindow::~StartMenuWindow()
{
	delete mHelpWindow;
	delete mGfxOptionWindow;
}


void StartMenuWindow::initMenu()
{
/*
	QPalette pal = mNewGame->palette( );
	pal.setColor( QPalette::Active, QPalette::Button, Qt::darkRed );
	pal.setColor( QPalette::Inactive, QPalette::Button, Qt::darkRed );
	pal.setColor( QPalette::ButtonText, Qt::white );
*/
	QFont buttonFont = font();
	buttonFont.setBold( true );
	buttonFont.setPixelSize( 24 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );

	mNewGame = new QPushButton( "New Game", this );
//	mNewGame->setPalette( pal );
	mNewGame->setFont( buttonFont );
	layout->addWidget( mNewGame );
	connect( mNewGame, SIGNAL( released() ), this, SLOT( handleNewGameButton() ) );

	mOptions = new QPushButton( "Options", this );
//	mOptions->setPalette( pal );
	mOptions->setFont( buttonFont );
	layout->addWidget( mOptions );
	connect( mOptions, SIGNAL( released() ), this, SLOT( handleOptionsButton() ) );

	mEnd = new QPushButton( "End Game", this );
//	mEnd->setPalette( pal );
	mEnd->setFont( buttonFont );
	layout->addWidget( mEnd );
	connect( mEnd, SIGNAL( released() ), this, SLOT( handleEndGameButton() ) );

	setLayout( layout );
}


void StartMenuWindow::handleNewGameButton()
{
	//TODO: implement
	this->setVisible( false );
}


void StartMenuWindow::handleOptionsButton()
{
	mGfxOptionWindow->show();
}


void StartMenuWindow::handleEndGameButton()
{
	QCoreApplication::exit(0);
}
