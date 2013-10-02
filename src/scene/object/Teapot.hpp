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

#ifndef SCENE_OBJECT_TEAPOT_INCLUDED
#define SCENE_OBJECT_TEAPOT_INCLUDED

#include "AObject.hpp"


class Material;
class StaticModel;
class AudioSample;


/// It's a Teapot
/**
 *
 */
class Teapot : public AObject
{
public:
	Teapot( Scene * scene, const float & size );
	virtual ~Teapot();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual QVector<const AObject*> collideSphere( const AObject * exclude, const float & radius, QVector3D & center, QVector3D * normal = NULL ) const;

private:
	Material * mMaterial;
	StaticModel * mModel;
	AudioSample * mAudioSample;
	float mSize;
	QVector3D mLastWorldPosition;
};


#endif
