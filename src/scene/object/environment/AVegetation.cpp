#include "AVegetation.hpp"

#include <QSettings>

int AVegetation::sQuality = 0;

AVegetation::AVegetation( World * world, int priority , float boundingSphereRadius) :
	AWorldObject( world, boundingSphereRadius ),
	mPriority( priority )
{
}
