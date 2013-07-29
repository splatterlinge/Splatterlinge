#ifndef UTILITY_BILBOARD_INCLUDED
#define UTILITY_BILBOARD_INCLUDED

#include <GLWidget.hpp>


/// Screen-align geometry
/**
 *
 */
namespace Bilboard
{
	inline QMatrix4x4 matrix( const QMatrix4x4 & modelView )
		{ QMatrix4x4 bilboard; bilboard.setColumn( 3, modelView.column(3) ); return bilboard; }

	inline void begin( const QMatrix4x4 & modelView )
		{ glPushMatrix(); glLoadMatrix( matrix( modelView ) ); }

	inline void end()
		{ glPopMatrix(); }
}


#endif
