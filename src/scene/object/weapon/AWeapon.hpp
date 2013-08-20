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

#ifndef SCENE_OBJECT_WEAPON_AWEAPON_INCLUDED
#define SCENE_OBJECT_WEAPON_AWEAPON_INCLUDED


#include "../AWorldObject.hpp"


/// Abstract baseclass for a weapon
/**
 *
 */
class AWeapon : public AWorldObject
{
public:
	/// Creates a weapon for the given world
	AWeapon( World * world );
	AWeapon( World * world, int ammo, int ammoclip, int clipsize );
	/// Frees this object
	virtual ~AWeapon();

	virtual void triggerPressed() = 0;
	virtual void triggerReleased() = 0;
	virtual void holster() = 0;
	virtual void pull() = 0;
	virtual void reload();

	const QString name() const { return mName; }
	const int ammo() const { return mAmmo; }
	const int ammoclip() const { return mAmmoClip; }
	const int clipsize() const { return mClipSize; }

protected:
	QString mName;
	int mAmmo;
	int mAmmoClip;
	int mClipSize;
};


#endif
