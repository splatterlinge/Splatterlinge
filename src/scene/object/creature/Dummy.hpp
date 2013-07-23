#ifndef SCENE_OBJECT_CREATURE_DUMMY_INCLUDED
#define SCENE_OBJECT_CREATURE_DUMMY_INCLUDED


#include "ACreature.hpp"


struct GLUquadric;


/// Dummy creature
class Dummy : public ACreature
{
public:
	Dummy( World * world );
	~Dummy();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual void receiveDamage( int damage, const QVector3D & direction );

private:
	GLUquadric * mQuadric;
	double mTimeToOrientationChange;
	QVector3D mTarget;
};


#endif
