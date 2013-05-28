#ifndef UTILITY_INTERPOLATE_INCLUDED
#define UTILITY_INTERPOLATE_INCLUDED

#include <math.h>

///Linear interpolation between a and b.
template< class T > inline T interpolateLinear( T a, T b, double w ) {return  a*(1.0-w)+b*w;}
///Linear interpolation between a and b.
template< class T > inline T interpolateLinear( T a, T b, float w ) {return  a*(1.0f-w)+b*w;}

///Cosine interpolation between a and b.
template< class T > inline T interpolateCosine( T a, T b, float w) {float f=(1.0f-cosf(w*(float)M_PI))*0.5f; return a*(1.0f-f)+b*f;}
///Cosine interpolation between a and b.
template< class T > inline T interpolateCosine( T a, T b, double w) {double f=(1.0-cos(w*M_PI))*0.5; return a*(1.0-f)+b*f;}


#endif
