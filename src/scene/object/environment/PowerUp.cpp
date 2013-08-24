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


PowerUp::PowerUp( Landscape * landscape, const QPoint & mapPosition ) :
	AWorldObject( landscape->world() )
{
	setPositionX( mapPosition.x() );
	setPositionY( landscape->terrain()->getHeight( mapPosition )+1.5 );
	setPositionZ( mapPosition.y() );

	setBoundingSphere( 1.0f );
}


PowerUp::~PowerUp()
{
}


void PowerUp::updateSelf( const double &delta )
{
	mRotation += delta * 50;
}


void PowerUp::drawSelf()
{
	glPushMatrix();

	glEnable( GL_COLOR_MATERIAL );
	glDisable( GL_CULL_FACE );

	glColor3f( 1.0f, 0.0f, 0.0f );
	glRotatef( mRotation, 0.0f, 1.0f, 0.0f );
	glScalef( 0.7f, 0.7f, 0.7f );
	glBegin( GL_QUADS );
	glVertex3f( -1.0f,  1.0f, 0.0 );
	glVertex3f( -1.0f, -1.0f, 0.0 );
	glVertex3f(  1.0f, -1.0f, 0.0 );
	glVertex3f(  1.0f,  1.0f, 0.0 );
	glEnd();

	glPopMatrix();
}
