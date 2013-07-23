#ifndef SCENE_OBJECT_CREATURE_ACREATURE_INCLUDED
#define SCENE_OBJECT_CREATURE_ACREATURE_INCLUDED


#include "../AWorldObject.hpp"


/// Abstract baseclass for a creature
/**
 *
 */
class ACreature : public AWorldObject
{
public:
	enum State
	{
		SPAWNING,
		ALIVE,
		DYING,
		DEAD
	};

	/// Creates a creature
	ACreature( World * world );
	/// Frees this creature
	virtual ~ACreature();

	const State & state() const { return mState; }

	const int & life() const { return mLife; }
	void setLife( const int & life ) { mLife = life; }

	virtual void receiveDamage( int damage, const QVector3D * position=NULL, const QVector3D * direction=NULL )
		{ mLife -= damage; if( mLife<0 ) mLife=0; };

protected:
	void setState( const State & state ) { mState = state; }

private:
	State mState;
	int mLife;
};


#endif
