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

#ifndef SCENE_OBJECT_WEAPON_MINIGUN_INCLUDED
#define SCENE_OBJECT_WEAPON_MINIGUN_INCLUDED

#include <qmath.h>
#include "AWeapon.hpp"
#include "resource/AudioSample.hpp"

class Material;
struct GLUquadric;


/// Caution - Laser
class Minigun : public AWeapon
{
public:
	Minigun( World * world );
	~Minigun();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void draw2Self();

	virtual void triggerPressed();
	virtual void triggerReleased();
	virtual void holster();
	virtual void pull();
	virtual void reload();
	virtual void setTarget( const QVector3D * target );

private:
	void spinUp( const double & delta );
	void spinDown( const double & delta );
	void reloadClip();

	GLUquadric * mQuadric;
	bool mDrawn;
	bool mFired;
	bool mReload;
	float mCoolDown;
	float mRange;
	float mTrailRadius;
	float mTrailLength;
	float mDamage;
	float mRotation;
	double mRPM;
	const QVector3D * mTarget;
	QVector3D mTrailStart;
	QVector3D mTrailDirection;
	QVector3D mTrailEnd;
	Material * mMaterial;
	AudioSample * mFireSound;
};


#endif
