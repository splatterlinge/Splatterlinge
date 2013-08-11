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

#include "Teapot.hpp"
#include "World.hpp"

#include <scene/Scene.hpp>
#include <geometry/teapot.h>
#include <resource/Material.hpp>
#include <resource/StaticModel.hpp>
#include <resource/AudioSample.hpp>
#include <utility/Sphere.hpp>
#include <utility/Capsule.hpp>


Teapot::Teapot( Scene * scene, const float & size ) :
	AObject( scene ),
	mSize( size )
{
	mMaterial = new Material( scene->glWidget(), "KirksEntry" );

//	mModel = new StaticModel( scene, "data/object/teapot/teapot.obj" );

	mAudioSample = new AudioSample( "data/sound/test.ogg" );
	mAudioSample->setLooping( true );
	mAudioSample->setRolloffFactor( 0.1f );
	mAudioSample->play();
}


Teapot::~Teapot()
{
	delete mMaterial;
//	delete mModel;
	delete mAudioSample;
}


void Teapot::updateSelf( const double & delta )
{
	mAudioSample->setPositionAutoVelocity( worldPosition(), delta );
	World * world = dynamic_cast<World*>(scene()->root());
	if( world )
	{
		setRotation( QQuaternion::nlerp( rotation(), world->landscape()->terrain()->getNormalRotation(position()), 33.0*delta ) );
	}
}


void Teapot::drawSelf()
{
	mMaterial->bind();

	glPushMatrix();
	glTranslatef( 0, mSize*0.6, 0 );
	teapot( 6, mSize, GL_FILL );
	glPopMatrix();

//	mModel->draw();

	mMaterial->release();
}


QVector<AObject*> Teapot::collideSphere( const AObject * exclude, const float & radius, QVector3D & center, QVector3D * normal )
{
	QVector<AObject*> collides = AObject::collideSphere( exclude, radius, center, normal );
	float depth;
	QVector3D tmpNormal;
	/*
	if( Sphere::intersectSphere( position(), boundingSphereRadius(), center, radius, &tmpNormal, &depth ) )
	{
		collides.append( this );
		center += tmpNormal * depth;
		if( normal )
			*normal = tmpNormal;
	}
	*/
	if( Capsule::intersectSphere( position(), position()+QVector3D(0,2,0), boundingSphereRadius()/2.0f, center, radius, &tmpNormal, &depth ) )
	{
		collides.append( this );
		center += tmpNormal * depth;
		if( normal )
			*normal = tmpNormal;
	}

	return collides;
}
