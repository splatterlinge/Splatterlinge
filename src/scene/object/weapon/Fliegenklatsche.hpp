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

#ifndef SCENE_OBJECT_WEAPON_FLIEGENKLATSCHE_HPP
#define SCENE_OBJECT_WEAPON_FLIEGENKLATSCHE_HPP

#include "AWeapon.hpp"

class Material;
class StaticModel;

class Fliegenklatsche : public AWeapon
{
public:
	Fliegenklatsche( World * world );
	~Fliegenklatsche();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual void triggerPressed();
	virtual void triggerReleased();
	virtual void holster();
	virtual void pull();
	virtual bool isPulled() { return mDrawn; }

private:
	StaticModel * mModel;
	bool mDrawn;
	bool mFired;
	bool mHitting;
	float mCooldown;
	float mDirection;
	float mRotation;
	float mRange;
	float mTrailLength;
	float mDamage;
	const QVector3D * mTarget;
	QVector3D mTrailStart;
	QVector3D mTrailDirection;
	QVector3D mTrailEnd;
};

#endif // FLIEGENKLATSCHE_HPP
