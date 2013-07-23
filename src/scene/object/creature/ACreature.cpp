#include "ACreature.hpp"


ACreature::ACreature( World * world ) :
	AWorldObject( world ),
	mState( ACreature::SPAWNING )
{
}


ACreature::~ACreature()
{
}
