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

#include "AWeapon.hpp"


AWeapon::AWeapon( World * world ) :
	AWorldObject::AWorldObject( world )
{
}


AWeapon::AWeapon( World *world, int ammo, int ammoclip, int clipsize ) :
	AWorldObject::AWorldObject( world ),
	mAmmo( ammo ),
	mAmmoClip( ammoclip ),
	mClipSize( clipsize )
{
}


AWeapon::~AWeapon()
{
}


void AWeapon::reload()
{
	if( mAmmoClip < mClipSize )
	{
		if( mAmmo > mClipSize )
		{
			mAmmoClip = mClipSize;
			mAmmo -= mClipSize;
		}
		else
		{
			mAmmoClip = mAmmo;
			mAmmo -= mAmmo;
		}
	}
}
