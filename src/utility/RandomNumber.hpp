#ifndef UTILITY_RANDOMNUMBER_INCLUDED
#define UTILITY_RANDOMNUMBER_INCLUDED

#include <stdlib.h>
#include <QtGlobal>


namespace RandomNumber
{
	inline float minMax( const float & min, const float & max)
		{ return ( (float)qrand()/(float)RAND_MAX ) * (max-min) + min; }
}


#endif
