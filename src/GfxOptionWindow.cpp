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

	materialQualityLabel = new QLabel();
	materialQuality = new QSlider( Qt::Horizontal );
	materialQuality->setRange( 0, 2 );
	materialQuality->setSingleStep( 1 );
	materialQuality->setPageStep( 1 );
	materialQuality->setTickPosition( QSlider::TicksAbove );
	materialQuality->setValue( MaterialQuality::maximum() );
	setMaterialQuality( materialQuality->value() );
	QObject::connect( materialQuality, SIGNAL(valueChanged(int)), this, SLOT(setMaterialQuality(int)) );
	layout->addWidget( materialQualityLabel );
	layout->addWidget( materialQuality );

	materialAnisotropyLabel = new QLabel();
	materialAnisotropy = new QSlider( Qt::Horizontal );
	materialAnisotropy->setRange( 1, MaterialQuality::filterAnisotropyMaximum() );
	materialAnisotropy->setSingleStep( 1 );
	materialAnisotropy->setPageStep( 1 );
	materialAnisotropy->setTickPosition( QSlider::TicksAbove );
	materialAnisotropy->setValue( 1.0f );
	setMaterialFilterAnisotropy( materialAnisotropy->value() );
	QObject::connect( materialAnisotropy, SIGNAL(valueChanged(int)), this, SLOT(setMaterialFilterAnisotropy(int)) );
	layout->addWidget( materialAnisotropyLabel );
	layout->addWidget( materialAnisotropy );

	farPlaneLabel = new QLabel();
	farPlane = new QSlider( Qt::Horizontal );
	farPlane->setRange( 50, 1000 );
	farPlane->setSingleStep( 10 );
	farPlane->setPageStep( 50 );
	farPlane->setTickPosition( QSlider::TicksAbove );
	farPlane->setValue( mScene->eye()->farPlane() );
	setFarPlane( farPlane->value() );
	QObject::connect( farPlane, SIGNAL(valueChanged(int)), this, SLOT(setFarPlane(int)) );
	layout->addWidget( farPlaneLabel );
	layout->addWidget( farPlane );

	mMultiSample = new QCheckBox( "MultiSample" );
	QObject::connect( mMultiSample, SIGNAL(stateChanged(int)), this, SLOT(setMultiSample(int)) );
	layout->addWidget( mMultiSample );

	setLayout( layout );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
}


GfxOptionWindow::~GfxOptionWindow()
{
}


void GfxOptionWindow::setMaterialQuality( int q )
{
	QString quality="";
	switch( q )
	{
		case MaterialQuality::HIGH:
			MaterialQuality::setMaximum( MaterialQuality::HIGH );
			quality = "High";
			break;
		case MaterialQuality::MEDIUM:
			MaterialQuality::setMaximum( MaterialQuality::MEDIUM );
			quality = "Medium";
			break;
		case MaterialQuality::LOW:
			MaterialQuality::setMaximum( MaterialQuality::LOW );
			quality = "Low";
			break;
	}
	materialQualityLabel->setText(tr("Material Quality (%1):").arg(quality));
}


void GfxOptionWindow::setMaterialFilterAnisotropy( int a )
{
	MaterialQuality::setFilterAnisotropy( a );
	materialAnisotropyLabel->setText(tr("Material Anisotropy (%1):").arg(a));
}


void GfxOptionWindow::setFarPlane( int distance )
{
	mScene->eye()->setFarPlane( distance );
	farPlaneLabel->setText(tr("Viewing Distance (%1m):").arg(distance));
}


void GfxOptionWindow::setMultiSample( int enable )
{
	mScene->setMultiSample( enable );
}
