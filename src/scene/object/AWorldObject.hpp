#ifndef SCENE_OBJECT_AWORLDOBJECT_INCLUDED
#define SCENE_OBJECT_AWORLDOBJECT_INCLUDED


#include "AObject.hpp"


class World;


/// Abstract baseclass for an object belonging to a world
/**
 *
 */
class AWorldObject : public AObject
{
public:
	/// Creates a weapon for the given world
	AWorldObject( World * world, float boundingSphereRadius=0.0f );
	/// Frees this object
	virtual ~AWorldObject();

	World * world() { return mWorld; }

private:
	World * mWorld;
};


#endif
