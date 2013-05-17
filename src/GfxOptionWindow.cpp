#include "GfxOptionWindow.hpp"

#include <resource/Material.hpp>
#include <scene/Scene.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>


GfxOptionWindow::GfxOptionWindow( Scene * scene, QWidget * parent, Qt::WindowFlags f ) :
	QWidget( parent, f ),
	mScene( scene )
{
	setWindowTitle( tr("Graphics Options") );
	setWindowOpacity( 0.8 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );

	mWireFrame = new QCheckBox( "WireFrame" );
	QObject::connect( mWireFrame, SIGNAL(stateChanged(int)), this, SLOT(setWireFrame(int)) );
	layout->addWidget( mWireFrame );

	mBoundingSpheres = new QCheckBox( "BoundingSpheres" );
	QObject::connect( mBoundingSpheres, SIGNAL(stateChanged(int)), this, SLOT(setBoundingSpheres(int)) );
	layout->addWidget( mBoundingSpheres );

	layout->addWidget( new QLabel("Material Quality:") );
	materialQuality = new QSlider( Qt::Horizontal );
	materialQuality->setMinimum( 0 );
	materialQuality->setMaximum( 2 );
	materialQuality->setValue( Material::globalMaxQuality() );
	QObject::connect( materialQuality, SIGNAL(valueChanged(int)), this, SLOT(setMaterialQuality(int)) );
	layout->addWidget( materialQuality );

	setLayout( layout );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
}


GfxOptionWindow::~GfxOptionWindow()
{
}


void GfxOptionWindow::setWireFrame( int enable )
{
	mScene->setWireFrame( enable );
}


void GfxOptionWindow::setBoundingSpheres( int enable )
{
	AObject::setGlobalDebugBoundingSpheres( enable );
}


void GfxOptionWindow::setMaterialQuality( int q )
{
	switch( q )
	{
		case Material::HIGH_QUALITY:
			Material::setGlobalMaxQuality( Material::HIGH_QUALITY );
			break;
		case Material::MEDIUM_QUALITY:
			Material::setGlobalMaxQuality( Material::MEDIUM_QUALITY );
			break;
		case Material::LOW_QUALITY:
			Material::setGlobalMaxQuality( Material::LOW_QUALITY );
			break;
	}
}
