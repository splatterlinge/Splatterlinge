#include "ACreature.hpp"


ACreature::ACreature( World * world ) :
	AWorldObject( world ),
	mState( ACreature::SPAWNING ),
	mLife( 0 )
{
}


ACreature::~ACreature()
{
}
