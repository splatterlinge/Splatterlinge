#ifndef GEOMETRY_PARTICLESYSTEM_INCLUDED
#define GEOMETRY_PARTICLESYSTEM_INCLUDED

#include <QVector>
#include <QVector2D>
#include <QVector3D>


class ParticleSystem
{
public:
	ParticleSystem( int capacity=1000 );
	~ParticleSystem() {}
	void update( const double & delta );
	void draw();
	void emitSpherical( const QVector3D & source, int toEmit, const float & minVel, const float & maxVel );
	void setMinLife( float minLife ) { mMinLife = minLife; }
	void setMaxLife( float maxLife ) { mMaxLife = maxLife; }
	void setDrag( float drag ) { mDrag = drag; }
	void setSize( float size ) { mSize = size; }
	void setGravity( QVector3D gravity ) { mGravity = gravity; }
	void setCapacity( int capacity ) { mParticles.resize( capacity ); mParticleVertices.resize( capacity*4 ); }

protected:

private:
	class Particle
	{
	public:
		Particle( float _life=0.0f, QVector3D _position=QVector3D(0,0,0), QVector3D _velocity=QVector3D(0,0,0) ) :
			velocity(_velocity), position(_position), life(_life) {}
		~Particle() {}
		QVector3D velocity;
		QVector3D position;
		float life;
		int rotate;
	};
	class ParticleVertex
	{
	public:
		QVector3D position;
		QVector3D normal;
		QVector2D coord;
	};
	float mMinLife;
	float mMaxLife;
	float mDrag;
	float mSize;
	QVector3D mGravity;
	QVector<Particle> mParticles;
	QVector<ParticleVertex> mParticleVertices;
};


#endif
