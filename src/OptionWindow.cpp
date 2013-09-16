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

#include "OptionWindow.hpp"

#include <resource/Material.hpp>
#include <scene/object/environment/AVegetation.hpp>
#include <scene/object/Landscape.hpp>
#include <scene/Scene.hpp>
#include <scene/object/World.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QSettings>
#include <QTabBar>


OptionWindow::OptionWindow( Scene * scene, QWidget * parent ) :
	QTabWidget( parent ),
	mScene( scene )
{
	setObjectName( "options" );
	setWindowTitle( tr("Options") );
	setWindowOpacity( 0.8 );

	setMinimumWidth( 300 );
	setMinimumHeight( 400 );
}


OptionWindow::~OptionWindow()
{
}
