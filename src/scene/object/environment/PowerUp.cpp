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

#include "PowerUp.hpp"

#include "../weapon/Laser.hpp"
#include "../weapon/Minigun.hpp"

#include <scene/Scene.hpp>


PowerUp::PowerUp( Landscape * landscape, QString type, const QPoint & mapPosition, int mapRadius ) :
	AWorldObject( landscape->world() ),
	mLandscape( landscape ),
	mPosition( mapPosition ),
	mRadius( mapRadius ),
	mRotationAngle( 0.0f )
{
	respawn();

	setBoundingSphere( 1.0f );

	if( type == "health" )
	{
		mPowerType = HEALTH;
		mMaterial = new Material( landscape->scene()->glWidget(), "PowerUp_Health" );
	}
	else if( type == "armor" )
	{
		mPowerType = ARMOR;
		mMaterial = new Material( landscape->scene()->glWidget(), "PowerUp_Armor" );
	}
	else if( type == "weapon_laser" )
	{
		mPowerType = WEAPON_LASER;
		mMaterial = new Material( landscape->scene()->glWidget(), "PowerUp_Weapon_Laser" );
	}
	else if( type == "weapon_minigun" )
	{
		mPowerType = WEAPON_MINIGUN;
		mMaterial = new Material( landscape->scene()->glWidget(), "PowerUp_Weapon_Minigun" );
	}
}


PowerUp::~PowerUp()
{
	delete mMaterial;
}


void PowerUp::respawn()
{
	QVector2D random = RandomNumber::inUnitCircle();
	QVector3D pos( mPosition.x() + random.x() * mRadius, 0, mPosition.y() + random.y() * mRadius );
	pos.setY( mLandscape->terrain()->getHeight( pos ) + 1.5 );

	setPosition( pos );
	mRespawnCoolDown = RandomNumber::minMax( 1.0f, 3.0f );
	mRespawning = true;
}


void PowerUp::updateSelf( const double & delta )
{
	mRotationAngle += delta * 100.0f;
	setRotation( QQuaternion::fromAxisAndAngle( 0,1,0, mRotationAngle ) );

	QSharedPointer<Player> player = mLandscape->world()->player();

	float dist = ( player->worldPosition() - worldPosition() ).length();

	if( mRespawning )
	{
		mRespawnCoolDown -= delta;
		if( mRespawnCoolDown < 0.0f )
		{
			mRespawnCoolDown = 0.0f;
			mRespawning = false;
		}
	}
	else
	{
		if( dist <= 2.0f )
		{
			switch( mPowerType )
			{
				case HEALTH:
					player->setLife( qMin( player->life() + 25, 100 ) );
					break;
				case ARMOR:
					player->setArmor( qMin( player->armor() + 40, 100 ) );
					break;
				case WEAPON_LASER:
					player->giveWeapon( QSharedPointer<AWeapon>( new Laser( world() ) ) );
					break;
				case WEAPON_MINIGUN:
					player->giveWeapon( QSharedPointer<AWeapon>( new Minigun( world() ) ) );
					break;
			}
			respawn();
		}
	}
}


void PowerUp::drawSelf()
{
	if( !mRespawning )
	{
		switch( mPowerType )
		{
			case HEALTH:
			case ARMOR:
			case WEAPON_LASER:
			case WEAPON_MINIGUN:
				glPushMatrix();

				glDisable( GL_CULL_FACE );

				mMaterial->bind();

				glColor3f( 1.0f, 0.0f, 0.0f );
				glScalef( 0.7f, 0.7f, 0.7f );
				glBegin( GL_QUADS );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, 0.0 );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 0.0 );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f, 0.0 );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, 0.0 );
				glEnd();

				mMaterial->release();

				glPopMatrix();
				break;
		}
	}
}
