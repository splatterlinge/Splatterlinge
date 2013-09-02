#ifndef AVEGETATION_HPP
#define AVEGETATION_HPP

#include "../AWorldObject.hpp"

class AVegetation : public AWorldObject
{
	static int sQuality;

protected:
	int mPriority;

public:
	AVegetation( World * world, int priority, float boundingSphereRadius=0.0f );

	static int quality() { return sQuality; }
	static void setQuality( int quality ) { sQuality = quality; }
};

#endif // AVEGETATION_HPP
