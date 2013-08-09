/*
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILITY_ALWRAPPERS_INCLUDED
#define UTILITY_ALWRAPPERS_INCLUDED

#include <AL/al.h>
#include <AL/alc.h>

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


QString alGetErrorString( ALenum error );


#endif
