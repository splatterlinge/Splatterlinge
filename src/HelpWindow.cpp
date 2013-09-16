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

#include "HelpWindow.hpp"

#include <resource/Material.hpp>
#include <scene/Scene.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QSpacerItem>


HelpWindow::HelpWindow( QWidget * parent ) :
	QWidget( parent )
{
	setWindowTitle( tr("Help") );
	setWindowOpacity( 0.8 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	layout->addWidget( new QLabel("Controls:\n\nW: Walk forward\nS: Walk backward\nA: Walk left\nD: Walk right\n\nMouse: View\nLeft Mouse: Fire") );
	layout->addSpacerItem( new QSpacerItem( 50, 1, QSizePolicy::Expanding, QSizePolicy::Expanding ) );
	setLayout( layout );
}


HelpWindow::~HelpWindow()
{
}
