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

#include "Grass.hpp"

#include <scene/object/Eye.hpp>
#include <scene/object/World.hpp>
#include <scene/object/Landscape.hpp>
#include <utility/Capsule.hpp>
#include <utility/Sphere.hpp>


Grass::Grass( Landscape * landscape, const QString & filename, const QPoint & mapPosition, int mapRadius, int number, int priority ) :
	AVegetation( landscape->world(), priority ),
	mLandscape( landscape )
{
	QPointF position = mLandscape->terrain()->fromMap( mapPosition );
	QSizeF radi = mLandscape->terrain()->fromMap( QSize( mapRadius, mapRadius ) );

	mModel = new StaticModel( world()->scene()->glWidget(), filename );
	setPosition( QVector3D( position.x(), 0, position.y() ) );
	setBoundingSphere( qMax( radi.width(),radi.height() ) );

	for( int i=0; i<number; i++ )
	{
		QVector3D treePos;
		int tries = 0;
		do {
			QVector2D random = RandomNumber::inUnitCircle();
			treePos.setX( position.x() + random.x() * radi.width() );
			treePos.setZ( position.y() + random.y() * radi.height() );
			treePos.setY( mLandscape->terrain()->getHeight( QPointF( treePos.x(),treePos.z()) ) - 1.0f );
			if( tries > 1000 )
			{
				qWarning() << QObject::tr("Giving up placing grass - no suitable position found");
				return;
			}
		} while( treePos.y() < mLandscape->waterHeight() );

		QMatrix4x4 pos;
		pos.translate( treePos );
		pos.scale( RandomNumber::minMax( 0.4f, 0.7f ) );
		pos.rotate( mLandscape->terrain()->getNormalRotation( treePos ) );

		mInstances.append(pos);
	}
}


Grass::~Grass()
{
	delete mModel;
}


void Grass::updateSelf( const double & delta )
{
}


void Grass::drawSelf()
{
	if( mPriority >= 99-quality() )
		mModel->draw( scene()->eye()->viewMatrix(), mInstances );
}
