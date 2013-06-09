#include "ParticleSystem.hpp"

#include <GLWidget.hpp>
#include <utility/random.hpp>

#include <math.h>


ParticleSystem::ParticleSystem( int capacity )
{
	setCapacity( capacity );
	mMinLife = 1.0f;
	mMaxLife = 2.0f;
	mDrag = 1.0f;
	mSize = 1.0f;
	mGravity = QVector3D( 0.0f, -9.81f, 0.0f );
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
		mParticles[i].position += mParticles[i].velocity * delta;
		mParticles[i].velocity *= powDragDelta;
		mParticles[i].velocity += deltaVelocity;
		mParticles[i].life -= delta;
	}
}


void ParticleSystem::draw()
{
	float m[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, m );
	QVector3D dir( m[2], m[6], m[10] );
	QVector3D up( m[1], m[5], m[9] );
	QVector3D right( m[0], m[4], m[8] );

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
		if( mParticles[i].life <= 0.0f )
			continue;

		int current = activeVertices;
		mParticleVertices[current].position = mParticles[i].position + vD;
		mParticleVertices[current].normal = nD;
		++current;
		mParticleVertices[current].position = mParticles[i].position + vC;
		mParticleVertices[current].normal = nC;
		++current;
		mParticleVertices[current].position = mParticles[i].position + vB;
		mParticleVertices[current].normal = nB;
		++current;
		mParticleVertices[current].position = mParticles[i].position + vA;
		mParticleVertices[current].normal = nA;

		current = activeVertices;
		int nextCoord = mParticles[i].rotate;
		for( int j = 0; j < 4; ++j )
		{
			switch( nextCoord & 0x03 )	// modulo 4
			{
			case 0:
				mParticleVertices[current].coord = QVector2D(0,0);
				break;
			case 1:
				mParticleVertices[current].coord = QVector2D(1,0);
				break;
			case 2:
				mParticleVertices[current].coord = QVector2D(1,1);
				break;
			case 3:
				mParticleVertices[current].coord = QVector2D(0,1);
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
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glClientActiveTexture( GL_TEXTURE0 );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glVertexPointer( 3, GL_FLOAT, sizeof(ParticleVertex), &(mParticleVertices.constData()->position) );
		glNormalPointer( GL_FLOAT, sizeof(ParticleVertex), &(mParticleVertices.constData()->normal) );
		glTexCoordPointer( 2, GL_FLOAT, sizeof(ParticleVertex), &(mParticleVertices.constData()->coord) );
		glDrawArrays( GL_QUADS, 0, activeVertices );
	}
}


void ParticleSystem::emitSpherical( const QVector3D & source, int toEmit, const float & minVel, const float & maxVel )
{
	for( int i=0; i<mParticles.size() && toEmit>0; ++i )
	{
		if( mParticles[i].life>0.0f )
			continue;

		--toEmit;
		QVector3D direction;
		do {
			direction = QVector3D( randomMinMax( -1.0, 1.0 ), randomMinMax( -1.0, 1.0 ), randomMinMax( -1.0, 1.0 ) );
		} while( direction.length() > 1.0f );
		direction.normalize();
		mParticles[i].velocity = direction * randomMinMax( minVel, maxVel );
		mParticles[i].position = source;
		mParticles[i].life = randomMinMax( mMinLife, mMaxLife );
		mParticles[i].rotate = rand()%4;	// for some variation
	}
}
