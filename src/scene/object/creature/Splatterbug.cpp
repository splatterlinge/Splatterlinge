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

#include "Splatterbug.hpp"
#include <scene/object/World.hpp>
#include <resource/Material.hpp>
#include <effect/SplatterSystem.hpp>
#include <utility/RandomNumber.hpp>
#include <utility/Quaternion.hpp>
#include <utility/Sphere.hpp>

#include <math.h>
#include <float.h>

Splatterbug::Splatterbug( World * world ) : ACreature( world )
{
	mBugSound = new AudioSample("butterfly");
}


Splatterbug::~Splatterbug()
{

}

void Splatterbug::updateSelf( const double & delta )
{
	
}

void Splatterbug::drawSelf()
{

}

AObject * Splatterbug::intersectLine( const AObject * exclude, const QVector3D & origin,
                                      const QVector3D & direction, float & length, 
                                      QVector3D * normal)
{
	AObject * nearestTarget = AObject::intersectLine( exclude, origin, direction, length, normal );

	return nearestTarget;
}

void Splatterbug::receiveDamage( int damage, const QVector3D * position,
                                 const QVector3D * direction)
{
	
}

bool Splatterbug::intersectBody(const QVector3D & origin, const QVector3D & direction, 
                                float & intersectionDistance)
{
	return true;
}

bool Splatterbug::intersectHead(const QVector3D & origin, const QVector3D & direction,
                                float & intersectionDistance)
{
	return true;
}
