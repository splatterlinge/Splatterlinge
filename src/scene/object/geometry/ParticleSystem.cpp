#include "ParticleSystem.hpp"

#include <GLWidget.hpp>
#include <utility/random.hpp>


ParticleSystem::ParticleSystem( int capacity ) :
	mParticles(QVector<Particle>(capacity))
{
	mMinLife = 1.0f;
	mMaxLife = 2.0f;
	mMass = 1.0f;
	mSize = 1.0f;
	mForce = QVector3D( 0.0f, -9.81f, 0.0f );
	for( int i=0; i<mParticles.size(); ++i )
	{
		mParticles[i] = Particle();
	}
}


void ParticleSystem::update( const float & delta )
{
	QVector3D deltaVelocity = mForce/mMass;
	for( int i=0; i<mParticles.size(); ++i )
	{
		mParticles[i].velocity += deltaVelocity;
		mParticles[i].position += mParticles[i].velocity * delta;
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
	QVector3D vA( -(right.x()   -up.x()) * mSize, -(right.y()   -up.y()) * mSize, -(right.z()   -up.z()) * mSize );
	QVector3D vB(  (right.x()   +up.x()) * mSize,  (right.y()   +up.y()) * mSize,  (right.z()   +up.z()) * mSize );
	QVector3D vC(  (right.x()   -up.x()) * mSize,  (right.y()   -up.y()) * mSize,  (right.z()   -up.z()) * mSize );
	QVector3D vD(  (  -up.x()-right.x()) * mSize,  (  -up.y()-right.y()) * mSize,  (  -up.z()-right.z()) * mSize );
	glBegin( GL_QUADS );
	glNormal( dir );
	for( int i=0; i<mParticles.size(); ++i )
	{
		if( mParticles[i].life <= 0.0f )
			continue;
		glTexCoord2i(0,0);	glVertex( mParticles[i].position + vA );
		glTexCoord2i(0,1);	glVertex( mParticles[i].position + vD );
		glTexCoord2i(1,1);	glVertex( mParticles[i].position + vC );
		glTexCoord2i(1,0);	glVertex( mParticles[i].position + vB );
	}
	glEnd();
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
	}
}
