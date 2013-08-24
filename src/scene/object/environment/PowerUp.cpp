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

#include "PowerUp.hpp"

#include <scene/Scene.hpp>


PowerUp::PowerUp( Landscape * landscape, QString type, const QPoint & mapPosition, int mapRadius ) :
	AWorldObject( landscape->world() ),
	mLandscape( landscape ),
	mPosition( mapPosition ),
	mRadius( mapRadius )
{
	mCoolDown = -1.0f;
	respawn();

	setBoundingSphere( 1.0f );

	if( type == "health" )
	{
		mPowerType = HEALTH;
		mMaterial = new Material( landscape->scene()->glWidget(), "PowerUp_Health" );
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
	mCoolDown = -1.0f;
}


void PowerUp::updateSelf( const double &delta )
{
	mRotation += delta * 50;

	float dist = ( mLandscape->world()->player()->worldPosition() - worldPosition() ).length();

	if( dist <= 1 && mCoolDown == -1.0f )
	{
		switch( mPowerType )
		{
			case HEALTH:
				mLandscape->world()->player()->receivePowerUp( HEALTH, 25 );
				mCoolDown = RandomNumber::minMax( 1.0f, 3.0f );
				break;
			case ARMOR:
				break;
		}
	}

	if( mCoolDown != -1.0f )
	{
		if( mCoolDown > delta )
			mCoolDown -= delta;
		else
			mCoolDown = 0.0f;
	}

	if( mCoolDown == 0.0f )
	{
		respawn();
	}
}


void PowerUp::drawSelf()
{
	if( mCoolDown == -1.0f )
	{
		switch( mPowerType )
		{
			case HEALTH:
			case ARMOR:
				glPushMatrix();

				glDisable( GL_CULL_FACE );

				mMaterial->bind();

				glColor3f( 1.0f, 0.0f, 0.0f );
				glRotatef( mRotation, 0.0f, 1.0f, 0.0f );
				glScalef( 0.7f, 0.7f, 0.7f );
				glBegin( GL_QUADS );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 0.0 );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 0.0 );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 0.0 );
				glNormal3f( 0.0f, 1.0f, 0.0f ); glTexCoord2f( 1.0f, 1.0f );	glVertex3f(  1.0f,  1.0f, 0.0 );
				glEnd();

				mMaterial->release();

				glPopMatrix();
				break;
		}
	}
}
