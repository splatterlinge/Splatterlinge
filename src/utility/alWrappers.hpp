#ifndef UTILITY_ALWRAPPERS_INCLUDED
#define UTILITY_ALWRAPPERS_INCLUDED

#include <AL/al.h>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QColor>
#include <QPointF>


/* TODO:
Q_STATIC_ASSERT(sizeof(QVector2D) == 2*sizeof(float));
Q_STATIC_ASSERT(sizeof(QVector3D) == 3*sizeof(float));
Q_STATIC_ASSERT(sizeof(QVector4D) == 4*sizeof(float));
Q_STATIC_ASSERT(sizeof(QPointF) == 2*sizeof(qreal));
*/


inline void alSource( const ALuint & source, const ALenum & param, const ALint & value )	{ alSourcei( source, param, value ); }
inline void alSource( const ALuint & source, const ALenum & param, const ALfloat & value )	{ alSourcef( source, param, value ); }
inline void alSourcev( const ALuint & source, const ALenum & param, const ALint * values )	{ alSourceiv( source, param, values ); }
inline void alSourcev( const ALuint & source, const ALenum & param, const ALfloat * values )	{ alSourcefv( source, param, values ); }
inline void alSource( const ALuint & source, const ALenum & param, const QVector3D & v )	{ alSourcev( source, param, reinterpret_cast<const float*>(&v) ); }
inline void alSource( const ALuint & source, const ALenum & param, const QVector4D & v )	{ alSourcev( source, param, reinterpret_cast<const float*>(&v) ); }

inline void alListener( const ALenum & param, const ALint & value )	{ alListeneri( param, value ); }
inline void alListener( const ALenum & param, const ALfloat & value )	{ alListenerf( param, value ); }
inline void alListenerv( const ALenum & param, const ALint * values )	{ alListeneriv( param, values ); }
inline void alListenerv( const ALenum & param, const ALfloat * values )	{ alListenerfv( param, values ); }
inline void alListener( const ALenum & param, const QVector3D & v )	{ alListenerv( param, reinterpret_cast<const float*>(&v) ); }
inline void alListener( const ALenum & param, const QVector4D & v )	{ alListenerv( param, reinterpret_cast<const float*>(&v) ); }


#endif
