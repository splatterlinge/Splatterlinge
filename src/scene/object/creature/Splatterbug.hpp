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

#ifndef _SPLATTERBUG_HPP_
#define _SPLATTERBUG_HPP_

#include "ACreature.hpp"
#include "resource/AudioSample.hpp"


struct GLUquadric;
class Material;


class Splatterbug: public ACreature 
{
public:

	Splatterbug( World * world );
	~Splatterbug();

	
	//Public methods
	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual AObject * intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction,
		float & length, QVector3D * normal = NULL );

	virtual void receiveDamage( int damage, const QVector3D * position = NULL, const QVector3D * direction = NULL );
	virtual void recalculateWingPosition( const double & delta );

	virtual bool intersectBody(const QVector3D & origin, const QVector3D & direction, float & intersectionDistance);
	virtual bool intersectRightWing(const QVector3D & origin, const QVector3D & direction, float & intersectionDistance);
	virtual bool intersectLeftWing(const QVector3D & origin, const QVector3D & direction, float & intersectionDistance);
	virtual bool intersectHead(const QVector3D & origin, const QVector3D & direction, float & intersectionDistance);

protected:
	;

private:
	;

};

#endif // _SPLATTERBUG_HPP_

