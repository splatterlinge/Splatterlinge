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

#include "GfxOptionWindow.hpp"

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


GfxOptionWindow::GfxOptionWindow( Scene * scene, QWidget * parent ) :
	QWidget( parent ),
	mScene( scene )
{
	setWindowTitle( tr("Graphics Options") );
	setWindowOpacity( 0.8 );

	mLayout = new QBoxLayout( QBoxLayout::TopToBottom, this );

	mSplatterQualityLabel = new QLabel();
	mSplatterQuality = new QSlider( Qt::Horizontal );
	mSplatterQuality->setRange( 0, SplatterQuality::num-1 );
	mSplatterQuality->setSingleStep( 1 );
	mSplatterQuality->setPageStep( 1 );
//	mSplatterQuality->setTickPosition( QSlider::TicksAbove );
	mSplatterQuality->setValue( SplatterQuality::maximum() );
	setSplatterQuality( mSplatterQuality->value() );
	QObject::connect( mSplatterQuality, SIGNAL(valueChanged(int)), this, SLOT(setSplatterQuality(int)) );
	mLayout->addWidget( mSplatterQualityLabel );
	mLayout->addWidget( mSplatterQuality );

	mMaterialQualityLabel = new QLabel();
	mMaterialQuality = new QSlider( Qt::Horizontal );
	mMaterialQuality->setRange( 0, MaterialQuality::num-1 );
	mMaterialQuality->setSingleStep( 1 );
	mMaterialQuality->setPageStep( 1 );
//	mMaterialQuality->setTickPosition( QSlider::TicksAbove );
	mMaterialQuality->setValue( MaterialQuality::maximum() );
	setMaterialQuality( mMaterialQuality->value() );
	QObject::connect( mMaterialQuality, SIGNAL(valueChanged(int)), this, SLOT(setMaterialQuality(int)) );
	mLayout->addWidget( mMaterialQualityLabel );
	mLayout->addWidget( mMaterialQuality );

	mLandscapeBlobPriorityLabel = new QLabel();
	mLandscapeBlobPriority = new QSlider( Qt::Horizontal );
	mLandscapeBlobPriority->setRange( 0, 99 );
	mLandscapeBlobPriority->setSingleStep( 1 );
	mLandscapeBlobPriority->setPageStep( 1 );
	mLandscapeBlobPriority->setValue( Landscape::Blob::quality() );
	setBlobQuality( mLandscapeBlobPriority->value() );
	QObject::connect( mLandscapeBlobPriority, SIGNAL(valueChanged(int)), this, SLOT(setBlobQuality(int)) );
	mLayout->addWidget( mLandscapeBlobPriorityLabel );
	mLayout->addWidget( mLandscapeBlobPriority );

	mLandscapeVegetationPriorityLabel = new QLabel();
	mLandscapeVegetationPriority = new QSlider( Qt::Horizontal );
	mLandscapeVegetationPriority->setRange( 0, 99 );
	mLandscapeVegetationPriority->setSingleStep( 1 );
	mLandscapeVegetationPriority->setPageStep( 1 );
	mLandscapeVegetationPriority->setValue( AVegetation::quality() );
	setVegetationQuality( mLandscapeVegetationPriority->value() );
	QObject::connect( mLandscapeVegetationPriority, SIGNAL(valueChanged(int)), this, SLOT(setVegetationQuality(int)) );
	mLayout->addWidget( mLandscapeVegetationPriorityLabel );
	mLayout->addWidget( mLandscapeVegetationPriority );

	mMaterialAnisotropyLabel = new QLabel();
	mMaterialAnisotropy = new QSlider( Qt::Horizontal );
	mMaterialAnisotropy->setRange( 1, Material::filterAnisotropyMaximum() );
	mMaterialAnisotropy->setSingleStep( 1 );
	mMaterialAnisotropy->setPageStep( 1 );
	//     materialAnisotropy->setTickPosition( QSlider::TicksAbove );
	mMaterialAnisotropy->setValue( Material::filterAnisotropy() );
	setMaterialFilterAnisotropy( mMaterialAnisotropy->value() );
	QObject::connect( mMaterialAnisotropy, SIGNAL(valueChanged(int)), this, SLOT(setMaterialFilterAnisotropy(int)) );
	mLayout->addWidget( mMaterialAnisotropyLabel );
	mLayout->addWidget( mMaterialAnisotropy );

	mFarPlaneLabel = new QLabel();
	mFarPlane = new QSlider( Qt::Horizontal );
	mFarPlane->setRange( 50, 1000 );
	mFarPlane->setSingleStep( 10 );
	mFarPlane->setPageStep( 50 );
//	mFarPlane->setTickPosition( QSlider::TicksAbove );
	mFarPlane->setValue( mScene->eye()->farPlane() );
	setFarPlane( mFarPlane->value() );
	QObject::connect( mFarPlane, SIGNAL(valueChanged(int)), this, SLOT(setFarPlane(int)) );
	mLayout->addWidget( mFarPlaneLabel );
	mLayout->addWidget( mFarPlane );

	mMultiSample = new QCheckBox( "MultiSample (needs restart)" );
	mMultiSample->setChecked( mScene->multiSample() );
	QObject::connect( mMultiSample, SIGNAL(stateChanged(int)), this, SLOT(setMultiSample(int)) );
	mLayout->addWidget( mMultiSample );

	mStereo = new QCheckBox( "Stereo Rendering" );
	mStereo->setChecked( mScene->stereo() );
	QObject::connect( mStereo, SIGNAL(stateChanged(int)), this, SLOT(setStereo(int)) );
	mLayout->addWidget( mStereo );

	mStereoUseOVR = new QCheckBox( "OVR" );
	mStereoUseOVR->setChecked( mScene->stereoUseOVR() );
	QObject::connect( mStereoUseOVR, SIGNAL(stateChanged(int)), this, SLOT(setStereoUseOVR(int)) );
	mLayout->addWidget( mStereoUseOVR );

	mLayout->addSpacerItem( new QSpacerItem( 50, 1, QSizePolicy::Expanding, QSizePolicy::Expanding ) );

	setLayout( mLayout );
}


GfxOptionWindow::~GfxOptionWindow()
{
	delete mStereo;
	delete mStereoUseOVR;
	delete mMultiSample;
	delete mFarPlaneLabel;
	delete mMaterialAnisotropy;
	delete mMaterialAnisotropyLabel;
	delete mMaterialQuality;
	delete mMaterialQualityLabel;
	delete mSplatterQuality;
	delete mSplatterQualityLabel;
	delete mLayout;
}


void GfxOptionWindow::setSplatterQuality( int q )
{
	SplatterQuality::Type quality = static_cast<SplatterQuality::Type>(q);
	SplatterQuality::setMaximum( quality );
	mSplatterQualityLabel->setText( tr("Splatter Quality (%1):").arg( SplatterQuality::toString(quality) ) );

	QSettings settings;
	settings.setValue( "splatterQuality", SplatterQuality::toString(quality) );
}


void GfxOptionWindow::setMaterialQuality( int q )
{
	MaterialQuality::Type quality = static_cast<MaterialQuality::Type>(q);
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


void GfxOptionWindow::setBlobQuality( int q )
{
	Landscape::Blob::setQuality( q );
	mLandscapeBlobPriorityLabel->setText(tr("Blob Quality (%1):").arg(q));

	QSettings settings;
	settings.setValue( "landscapeBlobQuality", q );
}


void GfxOptionWindow::setVegetationQuality( int q )
{
	AVegetation::setQuality( q );
	mLandscapeVegetationPriorityLabel->setText(tr("Vegetation Quality (%1):").arg(q));

	QSettings settings;
	settings.setValue( "landscapeVegetationQuality", q );
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


void GfxOptionWindow::setStereo( int state )
{
	bool enable = state;
	mScene->setStereo( enable );

	QSettings settings;
	settings.setValue( "stereo", enable );
}


void GfxOptionWindow::setStereoUseOVR( int state )
{
	bool enable = state;
	mScene->setStereoUseOVR( enable );

	QSettings settings;
	settings.setValue( "stereoUseOVR", enable );
}
