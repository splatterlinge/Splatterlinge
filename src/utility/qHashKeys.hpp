#ifndef UTILITY_QHASHKEYS_INCLUDED
#define UTILITY_QHASHKEYS_INCLUDED


#include <QHash>
#include <QPoint>


inline uint qHash( const QPoint & p )
{
	return qHash( QPair< int, int >( p.x(), p.y() ) );
}


#endif
