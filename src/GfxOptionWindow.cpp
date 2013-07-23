#include "GfxOptionWindow.hpp"

#include <resource/Material.hpp>
#include <scene/Scene.hpp>
#include <scene/object/World.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QSettings>


GfxOptionWindow::GfxOptionWindow( Scene * scene, QWidget * parent, Qt::WindowFlags f ) :
	QWidget( parent, f ),
	mScene( scene )
{
	setWindowTitle( tr("Graphics Options") );
	setWindowOpacity( 0.8 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );

	mSplatterQualityLabel = new QLabel();
	mSplatterQuality = new QSlider( Qt::Horizontal );
	mSplatterQuality->setRange( 0, SplatterQuality::num-1 );
	mSplatterQuality->setSingleStep( 1 );
	mSplatterQuality->setPageStep( 1 );
//	mSplatterQuality->setTickPosition( QSlider::TicksAbove );
	mSplatterQuality->setValue( SplatterQuality::maximum() );
	setSplatterQuality( mSplatterQuality->value() );
	QObject::connect( mSplatterQuality, SIGNAL(valueChanged(int)), this, SLOT(setSplatterQuality(int)) );
	layout->addWidget( mSplatterQualityLabel );
	layout->addWidget( mSplatterQuality );

	mMaterialQualityLabel = new QLabel();
	mMaterialQuality = new QSlider( Qt::Horizontal );
	mMaterialQuality->setRange( 0, MaterialQuality::num-1 );
	mMaterialQuality->setSingleStep( 1 );
	mMaterialQuality->setPageStep( 1 );
//	mMaterialQuality->setTickPosition( QSlider::TicksAbove );
	mMaterialQuality->setValue( MaterialQuality::maximum() );
	setMaterialQuality( mMaterialQuality->value() );
	QObject::connect( mMaterialQuality, SIGNAL(valueChanged(int)), this, SLOT(setMaterialQuality(int)) );
	layout->addWidget( mMaterialQualityLabel );
	layout->addWidget( mMaterialQuality );

	mMaterialAnisotropyLabel = new QLabel();
	mMaterialAnisotropy = new QSlider( Qt::Horizontal );
	mMaterialAnisotropy->setRange( 1, Material::filterAnisotropyMaximum() );
	mMaterialAnisotropy->setSingleStep( 1 );
	mMaterialAnisotropy->setPageStep( 1 );
//	materialAnisotropy->setTickPosition( QSlider::TicksAbove );
	mMaterialAnisotropy->setValue( Material::filterAnisotropy() );
	setMaterialFilterAnisotropy( mMaterialAnisotropy->value() );
	QObject::connect( mMaterialAnisotropy, SIGNAL(valueChanged(int)), this, SLOT(setMaterialFilterAnisotropy(int)) );
	layout->addWidget( mMaterialAnisotropyLabel );
	layout->addWidget( mMaterialAnisotropy );

	mFarPlaneLabel = new QLabel();
	mFarPlane = new QSlider( Qt::Horizontal );
	mFarPlane->setRange( 50, 1000 );
	mFarPlane->setSingleStep( 10 );
	mFarPlane->setPageStep( 50 );
//	mFarPlane->setTickPosition( QSlider::TicksAbove );
	mFarPlane->setValue( mScene->eye()->farPlane() );
	setFarPlane( mFarPlane->value() );
	QObject::connect( mFarPlane, SIGNAL(valueChanged(int)), this, SLOT(setFarPlane(int)) );
	layout->addWidget( mFarPlaneLabel );
	layout->addWidget( mFarPlane );

	mMultiSample = new QCheckBox( "MultiSample (needs restart)" );
	mMultiSample->setChecked( mScene->multiSample() );
	QObject::connect( mMultiSample, SIGNAL(stateChanged(int)), this, SLOT(setMultiSample(int)) );
	layout->addWidget( mMultiSample );

	setLayout( layout );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
}


GfxOptionWindow::~GfxOptionWindow()
{
}


void GfxOptionWindow::setSplatterQuality( int q )
{
	SplatterQuality::type quality = static_cast<SplatterQuality::type>(q);
	SplatterQuality::setMaximum( quality );
	mSplatterQualityLabel->setText( tr("Splatter Quality (%1):").arg( SplatterQuality::toString(quality) ) );

	QSettings settings;
	settings.setValue( "splatterQuality", SplatterQuality::toString(quality) );
}


void GfxOptionWindow::setMaterialQuality( int q )
{
	MaterialQuality::type quality = static_cast<MaterialQuality::type>(q);
	MaterialQuality::setMaximum( quality );
	mMaterialQualityLabel->setText( tr("Material Quality (%1):").arg( MaterialQuality::toString(quality) ) );

	QSettings settings;
	settings.setValue( "materialQuality", MaterialQuality::toString(quality) );
}


void GfxOptionWindow::setMaterialFilterAnisotropy( int a )
{
	Material::setFilterAnisotropy( a );
	mMaterialAnisotropyLabel->setText(tr("Material Anisotropy (%1):").arg(a));

	QSettings settings;
	settings.setValue( "materialFilterAnisotropy", a );
}


void GfxOptionWindow::setFarPlane( int distance )
{
	mScene->eye()->setFarPlane( distance );
	mFarPlaneLabel->setText(tr("Viewing Distance (%1m):").arg(distance));

	QSettings settings;
	settings.setValue( "farPlane", distance );
}


void GfxOptionWindow::setMultiSample( int state )
{
	bool enable = state;
	mScene->setMultiSample( enable );

	QSettings settings;
	settings.setValue( "sampleBuffers", enable );
}
