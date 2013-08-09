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

#include "HelpWindow.hpp"

#include <resource/Material.hpp>
#include <scene/Scene.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>


HelpWindow::HelpWindow( QWidget * parent, Qt::WindowFlags f ) :
	QWidget( parent, f )
{
	setWindowTitle( tr("Help") );
	setWindowOpacity( 0.8 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	layout->addWidget( new QLabel("F1\tThis Window\nF2\tGraphics Options\nF3\tDebug Options") );
	setLayout( layout );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
}


HelpWindow::~HelpWindow()
{
}
