#include "AWorldObject.hpp"
#include "World.hpp"


AWorldObject::AWorldObject( World * world, float boundingSphereRadius ) :
	AObject( world->scene(), boundingSphereRadius ),
	mWorld( world )
{
}


AWorldObject::~AWorldObject()
{
}
