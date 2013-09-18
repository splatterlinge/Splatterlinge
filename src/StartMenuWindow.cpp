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

#include "StartMenuWindow.hpp"

#include "OptionWindow.hpp"
#include "GfxOptionWindow.hpp"
#include "DebugWindow.hpp"
#include "HelpWindow.hpp"

#include <scene/Scene.hpp>

#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QListView>
#include <QStringList>
#include <QStringListModel>


StartMenuWindow::StartMenuWindow( Scene * scene, OptionWindow * options, QWidget * parent ) :
	QWidget( parent ),
	mScene( scene ),
	mOptionWindow( options )
{
	setObjectName( "startMenu" );
	setWindowTitle( tr( "Start Menu" ) );
	setWindowOpacity( 1.0 );

	initMenu();

	mHelpWindow = new HelpWindow();
	mHelpWindowScroll = new QScrollArea();
	mHelpWindowScroll->setWidget( mHelpWindow );
	mHelpWindowScroll->setWidgetResizable( true );

	mGfxOptionWindow = new GfxOptionWindow( mScene );
	mGfxOptionWindowScroll = new QScrollArea();
	mGfxOptionWindowScroll->setWidget( mGfxOptionWindow );
	mGfxOptionWindowScroll->setWidgetResizable( true );

	mDebugWindow = new DebugWindow( mScene );
	mDebugWindowScroll = new QScrollArea();
	mDebugWindowScroll->setWidget( mDebugWindow );
	mDebugWindowScroll->setWidgetResizable( true );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
}


StartMenuWindow::~StartMenuWindow()
{
}


void StartMenuWindow::initMenu()
{
/*
	QPalette pal = mNewGame->palette( );
	pal.setColor( QPalette::Active, QPalette::Button, Qt::darkRed );
	pal.setColor( QPalette::Inactive, QPalette::Button, Qt::darkRed );
	pal.setColor( QPalette::ButtonText, Qt::white );
*/
	QFont buttonFont = QFont( "Xolonium", 12, QFont::Normal );
	buttonFont.setBold( true );
	buttonFont.setPixelSize( 24 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );

	mNewGame = new QPushButton( "New Game", this );
//	mNewGame->setPalette( pal );
	mNewGame->setFont( buttonFont );
	layout->addWidget( mNewGame );
	connect( mNewGame, SIGNAL(released()), this, SLOT(handleNewGameButton()) );

	mOptions = new QPushButton( "Options", this );
//	mOptions->setPalette( pal );
	mOptions->setFont( buttonFont );
	layout->addWidget( mOptions );
	connect( mOptions, SIGNAL(released()), this, SLOT(handleOptionsButton()) );

	mEnd = new QPushButton( "End Game", this );
//	mEnd->setPalette( pal );
	mEnd->setFont( buttonFont );
	layout->addWidget( mEnd );
	connect( mEnd, SIGNAL(released()), this, SLOT(handleEndGameButton()) );

	setLayout( layout );
}


void StartMenuWindow::handleNewGameButton()
{
	//TODO: implement
	World * world = dynamic_cast<World*>(mScene->root());
	delete world;
	world = new World( mScene, "earth" );
	mScene->setRoot( world );

	this->setVisible( false );
	mScene->setPaused( false );
	mScene->setMouseGrabbing( true );
}


void StartMenuWindow::handleOptionsButton()
{
	if( !mOptionWindow->isVisible() )
	{
		mOptionWindow->clear();
		mOptionWindow->addTab( mHelpWindowScroll, "Help" );
		mOptionWindow->addTab( mGfxOptionWindowScroll, "Graphics" );
		mOptionWindow->addTab( mDebugWindowScroll, "Debug" );
		mOptionWindow->show();
	}
	else
		mOptionWindow->hide();
}


void StartMenuWindow::handleEndGameButton()
{
	QCoreApplication::quit();
}
