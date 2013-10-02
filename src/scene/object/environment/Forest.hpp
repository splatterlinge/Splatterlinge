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

#ifndef SCENE_OBJECT_ENVIRONMENT_FOREST_INCLUDED
#define SCENE_OBJECT_ENVIRONMENT_FOREST_INCLUDED

#include "AVegetation.hpp"
#include "../../Scene.hpp"

#include <QPointF>
#include <QVector>
#include <QMatrix4x4>


class Landscape;
class StaticModel;

class Forest : public AVegetation
{
public:
	Forest( Landscape * landscape, const QString & filename, const QPoint & mapPosition, int mapRadius, int number, int priority );
	virtual ~Forest();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual QVector<const AObject*> collideSphere( const AObject * exclude, const float & radius, QVector3D & center, QVector3D * normal ) const;

private:
	Landscape * mLandscape;
	QVector<QMatrix4x4> mInstances;
	StaticModel * mModel;
};


#endif
