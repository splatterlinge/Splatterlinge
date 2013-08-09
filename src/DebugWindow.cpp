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

#include "DebugWindow.hpp"

#include <scene/Scene.hpp>
#include <scene/object/Landscape.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>


DebugWindow::DebugWindow( Scene * scene, QWidget * parent, Qt::WindowFlags f ) :
	QWidget( parent, f ),
	mScene( scene )
{
	setWindowTitle( tr("Debug") );
	setWindowOpacity( 0.8 );

	mLayout = new QBoxLayout( QBoxLayout::TopToBottom, this );

	mWireFrame = new QCheckBox( "WireFrame" );
	QObject::connect( mWireFrame, SIGNAL(stateChanged(int)), this, SLOT(setWireFrame(int)) );
	mLayout->addWidget( mWireFrame );

	mObjectBoundingSpheres = new QCheckBox( "Bounding spheres for objects" );
	QObject::connect( mObjectBoundingSpheres, SIGNAL(stateChanged(int)), this, SLOT(setObjectBoundingSpheres(int)) );
	mLayout->addWidget( mObjectBoundingSpheres );

	setLayout( mLayout );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
}


DebugWindow::~DebugWindow()
{
	delete mLayout;
	delete mWireFrame;
	delete mObjectBoundingSpheres;
}


void DebugWindow::setWireFrame( int enable )
{
	mScene->setWireFrame( enable );
}


void DebugWindow::setObjectBoundingSpheres( int enable )
{
	AObject::setGlobalDebugBoundingSpheres( enable );
}
