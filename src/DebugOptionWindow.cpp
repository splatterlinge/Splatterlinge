#include "DebugOptionWindow.hpp"

#include <scene/Scene.hpp>
#include <scene/object/Landscape.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>


DebugOptionWindow::DebugOptionWindow( Scene * scene, QWidget * parent, Qt::WindowFlags f ) :
	QWidget( parent, f ),
	mScene( scene )
{
	setWindowTitle( tr("Debug Options") );
	setWindowOpacity( 0.8 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );

	mWireFrame = new QCheckBox( "WireFrame" );
	QObject::connect( mWireFrame, SIGNAL(stateChanged(int)), this, SLOT(setWireFrame(int)) );
	layout->addWidget( mWireFrame );

	mObjectBoundingSpheres = new QCheckBox( "Bounding spheres for objects" );
	QObject::connect( mObjectBoundingSpheres, SIGNAL(stateChanged(int)), this, SLOT(setObjectBoundingSpheres(int)) );
	layout->addWidget( mObjectBoundingSpheres );

	setLayout( layout );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
}


DebugOptionWindow::~DebugOptionWindow()
{
}


void DebugOptionWindow::setWireFrame( int enable )
{
	mScene->setWireFrame( enable );
}


void DebugOptionWindow::setObjectBoundingSpheres( int enable )
{
	AObject::setGlobalDebugBoundingSpheres( enable );
}
