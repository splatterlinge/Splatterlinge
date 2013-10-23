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
#include <scene/object/World.hpp>

#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include <QStackedWidget>


StartMenuWindow::StartMenuWindow( Scene * scene, QWidget * parent ) :
	QWidget( parent ),
	mScene( scene )
{
	setObjectName( "startMenu" );
	setWindowTitle( tr( "Start Menu" ) );
	setWindowOpacity( 1.0 );

	mLeftMenuSide = newButtonBox( this );
	mLeftMenuSide->setObjectName( "startMenuButtons" );

	mRightMenuSide = new QStackedWidget( this );

	mOptions = newOptionTabs( this );
	mOptions->setObjectName( "startMenuOptions" );
	mRightMenuSide->addWidget( mOptions );

	mHelp= new HelpWindow();
	mHelp->setObjectName( "startMenuHelp" );
	mRightMenuSide->addWidget( mHelp );

	mRightMenuSide->setCurrentWidget( mHelp );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::LeftToRight, this );
	layout->addWidget( mLeftMenuSide );
	layout->addWidget( mRightMenuSide );
	setLayout( layout );


	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
}


StartMenuWindow::~StartMenuWindow()
{
}


void StartMenuWindow::resizeEvent( QResizeEvent * event )
{
	QWidget::resizeEvent( event );
}


QWidget * StartMenuWindow::newButtonBox( QWidget * parent )
{
	QFont buttonFont = QFont( "Xolonium", 12, QFont::Normal );
	buttonFont.setBold( true );
	buttonFont.setPixelSize( 24 );

	QScrollArea * container = new QScrollArea( parent );
	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, container );

	QPushButton * newGameButton = new QPushButton( "New Game", this );
	newGameButton->setFont( buttonFont );
	layout->addWidget( newGameButton );
	connect( newGameButton, SIGNAL(released()), this, SLOT(handleNewGameButton()) );

	QPushButton * helpButton = new QPushButton( "Help", this );
	helpButton->setFont( buttonFont );
	layout->addWidget( helpButton );
	connect( helpButton, SIGNAL(released()), this, SLOT(handleHelpButton()) );

	QPushButton * optionsButton = new QPushButton( "Options", this );
	optionsButton->setFont( buttonFont );
	layout->addWidget( optionsButton );
	connect( optionsButton, SIGNAL(released()), this, SLOT(handleOptionsButton()) );

	QPushButton * endButton = new QPushButton( "End Game", this );
	endButton->setFont( buttonFont );
	layout->addWidget( endButton );
	connect( endButton, SIGNAL(released()), this, SLOT(handleEndGameButton()) );

	container->setLayout( layout );

	return container;
}


QWidget * StartMenuWindow::newOptionTabs( QWidget * parent )
{
	QTabWidget * optionTabs = new QTabWidget( parent );
	optionTabs->clear();

	GfxOptionWindow * gfxOptionWindow = new GfxOptionWindow( mScene, optionTabs );
	QScrollArea * gfxOptionWindowScroll = new QScrollArea();
	gfxOptionWindowScroll->setWidget( gfxOptionWindow );
	gfxOptionWindowScroll->setWidgetResizable( true );

	DebugWindow * debugOptionWindow = new DebugWindow( mScene, optionTabs );
	QScrollArea * debugOptionWindowScroll = new QScrollArea();
	debugOptionWindowScroll->setWidget( debugOptionWindow );
	debugOptionWindowScroll->setWidgetResizable( true );

	optionTabs->addTab( gfxOptionWindowScroll, "Graphics" );
	optionTabs->addTab( debugOptionWindowScroll, "Debug" );

	return optionTabs;
}


void StartMenuWindow::handleNewGameButton()
{
	//TODO: implement
	World * world = dynamic_cast<World*>(mScene->root());
	delete world;
	world = new World( mScene, "earth" );
	mScene->setRoot( world );
	mScene->toggleMenu();
}


void StartMenuWindow::handleHelpButton()
{
	mRightMenuSide->setCurrentWidget( mHelp );
}


void StartMenuWindow::handleOptionsButton()
{
	mRightMenuSide->setCurrentWidget( mOptions );
}


void StartMenuWindow::handleEndGameButton()
{
	QCoreApplication::quit();
}
