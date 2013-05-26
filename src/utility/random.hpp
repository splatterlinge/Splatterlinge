#ifndef UTILITY_RANDOM_INCLUDED
#define UTILITY_RANDOM_INCLUDED

#include <stdlib.h>
#include <QtGlobal>


inline float randomMinMax( const float & min, const float & max)
	{ return ( (float)qrand()/(float)RAND_MAX ) * (max-min) + min; }


#endif
