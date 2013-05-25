#ifndef SCENE_OBJECT_GEOMETRY_PARTICLESYSTEM_INCLUDED
#define SCENE_OBJECT_GEOMETRY_PARTICLESYSTEM_INCLUDED

#include <QVector>
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
	void setMass( float mass ) { mMass = mass; }
	void setDrag( float drag ) { mDrag = drag; }
	void setSize( float size ) { mSize = size; }
	void setForce( QVector3D force ) { mForce = force; }

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
	};

	float mMinLife;
	float mMaxLife;
	float mMass;
	float mDrag;
	float mSize;
	QVector<Particle> mParticles;
	QVector3D mForce;
};


#endif
