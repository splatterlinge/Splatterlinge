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


void GfxOptionWindow::setFarPlane( int distance )
{
	mScene->eye()->setFarPlane( distance );
	farPlaneLabel->setText(tr("Viewing Distance (%1m):").arg(distance));
}
