/*
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

#ifndef SCENE_OBJECT_CREATURE_ACREATURE_INCLUDED
#define SCENE_OBJECT_CREATURE_ACREATURE_INCLUDED


#include "../AWorldObject.hpp"


/// Abstract baseclass for a creature
/**
 *
 */
class ACreature : public AWorldObject
{
public:
	enum State
	{
		SPAWNING,
		ALIVE,
		DYING,
		DEAD
	};

	/// Creates a creature
	ACreature( World * world );
	/// Frees this creature
	virtual ~ACreature();

	const State & state() const { return mState; }

	const int & life() const { return mLife; }
	void setLife( const int & life ) { mLife = life; }

	virtual void receiveDamage( int damage, const QVector3D * position=NULL, const QVector3D * direction=NULL )
		{ mLife -= damage; if( mLife<0 ) mLife=0; };

protected:
	void setState( const State & state ) { mState = state; }

private:
	State mState;
	int mLife;
};


#endif
