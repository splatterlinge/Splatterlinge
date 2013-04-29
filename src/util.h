#ifndef MATH_INCLUDED
#define MATH_INCLUDED

#include <math.h>


///Linear interpolation between a and b.
inline float interpolateLinear(float a, float b, float w) {return  a*(1.0f-w)+b*w;}
///Linear interpolation between a and b.
inline double interpolateLinear(double a, double b, double w) {return  a*(1.0-w)+b*w;}

///Cosine interpolation between a and b.
inline float interpolateCosine(float a, float b, float w) {float f=(1.0f-cosf(w*(float)M_PI))*0.5f; return a*(1.0f-f)+b*f;}
///Cosine interpolation between a and b.
inline double interpolateCosine(double a, double b, double w) {double f=(1.0-cos(w*M_PI))*0.5; return a*(1.0-f)+b*f;}


#endif
