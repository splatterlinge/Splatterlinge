#ifndef SCENE_OBJECT_CREATURE_DUMMY_INCLUDED
#define SCENE_OBJECT_CREATURE_DUMMY_INCLUDED


#include "ACreature.hpp"


struct GLUquadric;
class Material;


/// Dummy creature
class Dummy : public ACreature
{
public:
	Dummy( World * world );
	~Dummy();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual AObject * intersectLine( const QVector3D & origin, const QVector3D & direction,
		float & length, QVector3D * normal = NULL );

	virtual void receiveDamage( int damage, const QVector3D * position=NULL, const QVector3D * direction=NULL );
private:
	GLUquadric * mQuadric;
	Material * mMaterial;
	QVector3D mTarget;
	float mVelocityY;
};


#endif
