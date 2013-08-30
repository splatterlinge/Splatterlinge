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

#include <utility/Quaternion.hpp>


AWeapon::AWeapon( World * world ) :
	AWorldObject::AWorldObject( world )
{
}


AWeapon::AWeapon( World *world, int ammo, int ammoclip, int clipsize ) :
	AWorldObject::AWorldObject( world ),
	mAmmo( ammo ),
	mClipAmmo( ammoclip ),
	mClipSize( clipsize )
{
}


QQuaternion AWeapon::getRotationToTarget( const QVector3D * target, const float & maxRotationDP )
{
	if( target )
	{
		QMatrix4x4 parentModelMatrix;
		if( parent() )
		{
			parentModelMatrix = parent()->modelMatrix();
		} else {
			parentModelMatrix.setToIdentity();
		}

		QVector3D localTarget = ( parentModelMatrix.inverted() * QVector4D(*target,1) ).toVector3D();
		QVector3D dirToLocalTarget = ( localTarget - position() ).normalized();

		if( QVector3D::dotProduct( dirToLocalTarget, QVector3D(0,0,1) ) < maxRotationDP )
			return QQuaternion();
		else
			return Quaternion::fromTo( QVector3D(0,0,1), dirToLocalTarget );
	}
	else
		return QQuaternion();
}


AWeapon::~AWeapon()
{
}
