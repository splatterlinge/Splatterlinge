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

#include "ParticleSystem.hpp"

#include <GLWidget.hpp>
#include <utility/RandomNumber.hpp>

#include <math.h>


ParticleSystem::ParticleSystem( int capacity )
{
	setCapacity( capacity );
	mMinLife = 1.0f;
	mMaxLife = 2.0f;
	mDrag = 1.0f;
	mSize = 1.0f;
	mGravity = QVector3D( 0.0f, -9.81f, 0.0f );
	mInteractionCallback = 0;
	for( int i=0; i<mParticles.size(); ++i )
	{
		mParticles[i] = Particle();
	}
}


void ParticleSystem::update( const double & delta )
{
	QVector3D deltaVelocity = mGravity * delta;
	double powDragDelta = pow( mDrag, delta );
	for( int i=0; i<mParticles.size(); ++i )
	{
		if( mParticles[i].life() <= 0.0f )
			continue;
		mParticles[i].rPosition() += mParticles[i].velocity() * delta;
		mParticles[i].rVelocity() *= powDragDelta;
		mParticles[i].rVelocity() += deltaVelocity;
		mParticles[i].rLife() -= delta;
		if( mInteractionCallback )
			mInteractionCallback->particleInteraction( delta, mParticles[i] );
	}
}


void ParticleSystem::draw( const QMatrix4x4 & modelView )
{
	QVector3D dir( modelView.row(2).toVector3D() );
	QVector3D up( modelView.row(1).toVector3D() );
	QVector3D right( modelView.row(0).toVector3D() );

	QVector3D vA = (-right+up) * mSize;
	QVector3D vB = ( right+up) * mSize;
	QVector3D vC = ( right-up) * mSize;
	QVector3D vD = (-right-up) * mSize;

	QVector3D nA = (vA+dir).normalized();
	QVector3D nB = (vB+dir).normalized();
	QVector3D nC = (vC+dir).normalized();
	QVector3D nD = (vD+dir).normalized();

	int activeVertices = 0;
	for( int i=0; i<mParticles.size(); ++i )
	{
		if( mParticles[i].life() <= 0.0f )
			continue;

		int current = activeVertices;
		mParticleVertices[current].position = mParticles[i].position() + vD;
		mParticleVertices[current].normal = nD;
		++current;
		mParticleVertices[current].position = mParticles[i].position() + vC;
		mParticleVertices[current].normal = nC;
		++current;
		mParticleVertices[current].position = mParticles[i].position() + vB;
		mParticleVertices[current].normal = nB;
		++current;
		mParticleVertices[current].position = mParticles[i].position() + vA;
		mParticleVertices[current].normal = nA;

		current = activeVertices;
		int nextCoord = mParticles[i].rotation();
		for( int j = 0; j < 4; ++j )
		{
			switch( nextCoord & 0x03 )	// modulo 4
			{
			case 0:
				mParticleVertices[current].texCoord = QVector2D(0,0);
				break;
			case 1:
				mParticleVertices[current].texCoord = QVector2D(1,0);
				break;
			case 2:
				mParticleVertices[current].texCoord = QVector2D(1,1);
				break;
			case 3:
				mParticleVertices[current].texCoord = QVector2D(0,1);
				break;
			}
			++nextCoord;
			++current;
		}
		activeVertices+=4;
	}
	if( activeVertices )
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

		glClientActiveTexture( GL_TEXTURE0 );
		VertexP3fN3fT2f::glEnableClientState();

		glVertexPointer( 3, GL_FLOAT, sizeof(VertexP3fN3fT2f), &(mParticleVertices.constData()->position) );
		glNormalPointer( GL_FLOAT, sizeof(VertexP3fN3fT2f), &(mParticleVertices.constData()->normal) );
		glTexCoordPointer( 2, GL_FLOAT, sizeof(VertexP3fN3fT2f), &(mParticleVertices.constData()->texCoord) );

		glDrawArrays( GL_QUADS, 0, activeVertices );

		VertexP3fN3fT2f::glDisableClientState();
	}
}


void ParticleSystem::emitSpherical( const QVector3D & source, int toEmit, const float & minVel, const float & maxVel )
{
	for( int i=0; i<mParticles.size() && toEmit>0; ++i )
	{
		if( mParticles[i].life()>0.0f )
			continue;

		--toEmit;
		QVector3D direction = RandomNumber::inUnitSphere();
		direction.normalize();
		mParticles[i].rVelocity() = direction * RandomNumber::minMax( minVel, maxVel );
		mParticles[i].rPosition() = source;
		mParticles[i].rLife() = RandomNumber::minMax( mMinLife, mMaxLife );
	}
}
