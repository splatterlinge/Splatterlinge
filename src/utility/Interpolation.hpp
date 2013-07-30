#ifndef UTILITY_INTERPOLATION_INCLUDED
#define UTILITY_INTERPOLATION_INCLUDED

#include <math.h>


namespace Interpolation
{
	///Linear interpolation between a and b.
	template< class T > inline T linear( const T & a, const T & b, const double & w )
		{ return a*(1.0-w)+b*w; }

	///Linear interpolation between a and b.
	template< class T > inline T linear( const T & a, const T & b, const float & w )
		{ return a*(1.0f-w)+b*w; }

	///Cosine interpolation between a and b.
	template< class T > inline T cosine( const T & a, const T & b, const double & w )
		{ double f=(1.0-cos(w*M_PI))*0.5; return a*(1.0-f)+b*f; }

	///Cosine interpolation between a and b.
	template< class T > inline T cosine( const T & a, const T & b, const float & w )
		{ float f=(1.0f-cosf(w*(float)M_PI))*0.5f; return a*(1.0f-f)+b*f; }
}


#endif
