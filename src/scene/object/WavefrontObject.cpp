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

#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject( Scene * scene, QString filename, QVector<QMatrix4x4> * instances ) :
	AObject( scene ),
	mScene( scene ),
	mFilename( filename ),
	mInstances( instances)
{
	mModel = new StaticModel( mScene, mFilename );

	setBoundingSphere( qMax( mModel->getSize().width(), mModel->getSize().height() ) );
}


WavefrontObject::~WavefrontObject()
{
	delete mModel;
	delete mInstances;
}


void WavefrontObject::updateSelf( const double & delta )
{
}


void WavefrontObject::drawSelf()
{
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_CULL_FACE );
	glEnable( GL_AUTO_NORMAL );
	glEnable( GL_NORMALIZE );
	glEnable( GL_LIGHTING );

	glPushMatrix();

	glColor3f( 1.0f, 1.0f, 1.0f );

	mModel->draw( mInstances );

	glPopMatrix();
	glPopAttrib();
}
