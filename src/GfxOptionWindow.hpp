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

#ifndef GFXOPTIONWINDOW_INCLUDED
#define GFXOPTIONWINDOW_INCLUDED


#include <QWidget>


class Scene;

class QLabel;
class QSlider;
class QCheckBox;
class QBoxLayout;


class GfxOptionWindow : public QWidget
{
	Q_OBJECT

public:
	GfxOptionWindow( Scene * scene, QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~GfxOptionWindow();

private:
	Scene * mScene;
	QBoxLayout * mLayout;
	QLabel * mSplatterQualityLabel;
	QSlider * mSplatterQuality;
	QLabel * mMaterialQualityLabel;
	QSlider * mMaterialQuality;
	QLabel * mMaterialAnisotropyLabel;
	QSlider * mMaterialAnisotropy;
	QLabel * mLandscapeBlobPriorityLabel;
	QSlider * mLandscapeBlobPriority;
	QLabel * mLandscapeVegetationPriorityLabel;
	QSlider * mLandscapeVegetationPriority;
	QLabel * mFarPlaneLabel;
	QSlider * mFarPlane;
	QCheckBox * mMultiSample;
	QCheckBox * mStereo;
	QCheckBox * mStereoUseOVR;

public slots:
	void setMaterialQuality( int q );
	void setSplatterQuality( int q );
	void setMaterialFilterAnisotropy( int a );
	void setBlobQuality( int q );
	void setVegetationQuality( int q );
	void setFarPlane( int distance );
	void setMultiSample( int state );
	void setStereo( int state );
	void setStereoUseOVR( int state );
};


#endif
